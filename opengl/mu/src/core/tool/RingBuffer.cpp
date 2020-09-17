#include "RingBuffer.h"

MU_BEGIN_NAMESPACE

const char* RingBuffer::readPointerAtPosition(int64_t pos, int64_t &len) const
{
    if (pos >= 0) {
        pos += mHead;
        int index = 0;
        for (rbuf_cit it = mBuffers.begin(); it != mBuffers.end() && size_t(index) < mBuffers.size(); ++it, ++index) {
            len = (index == mTailBuffer ? mTail : it->size());
            if (len > pos) {
                len -= pos;
                return it->data() + pos;
            }
            pos -= len;
        }
    }

    len = 0;
    return NULL;
}

int64_t RingBuffer::indexOf(char c, int64_t maxLen, int64_t pos) const
{
    if (maxLen <= 0 || pos < 0) {
        return -1;
    }

    int64_t index = -(pos + mHead);
    rbuf_cit it = mBuffers.begin();
    for (size_t i = 0; i < mBuffers.size() && it != mBuffers.end(); ++i, ++it) {
        int64_t nextBlockIndex = std::min(int(index + (i == size_t(mTailBuffer) ? mTail : it->size())), int(maxLen));
        if (nextBlockIndex > 0) {
            const char *ptr = it->data();
            if (index < 0) {
                ptr -= index;
                index = 0;
            }
            const char *findPtr = reinterpret_cast<const char*>(::memchr(ptr, c, nextBlockIndex - index));
            if (findPtr) {
                return int64_t(findPtr - ptr) + index + pos;
            }
            if (nextBlockIndex == maxLen) {
                return -1;
            }

        }
        index = nextBlockIndex;
    }

    return -1;
}

void RingBuffer::chop(int64_t bytes)
{
    MU_ASSERT2(bytes <= mBufferSize);

    while (bytes > 0) {
        if (0 == mTailBuffer || mTail > bytes) {
            if (mBufferSize <= bytes) {
                // keep a single block around
                // if it doesnot exceed the basic block size
                // to avoid repeated allocations between uses of the buffer
                if (mBuffers.front().size() <= size_t(mBasicBlockSize)) {
                    mHead = 0;
                    mTail = 0;
                    mBufferSize = 0;
                }
                else {
                    // try to minify/squeeze us
                    clear();
                }
            }
            else {
                MU_ASSERT2(bytes < RINGBUFFER_SINGLEBUFFER_MAXSIZE);
                mTail -= int(bytes);
                mBufferSize -= bytes;
            }
            return;
        }
        mBufferSize -= mTail;
        bytes -= mTail;
        mBuffers.pop_back();
        --mTailBuffer;
        mTail = mBuffers.back().size();
    }
}

char* RingBuffer::reserve(int64_t bytes)
{
    if (bytes <= 0 || bytes >= RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
        return NULL;
    }

    if (0 == mBufferSize) {
        if (mBuffers.empty()) {
            mBuffers.push_back(std::string(std::max(mBasicBlockSize, int(bytes)), 0));
        }
        else {
            mBuffers.front().resize(std::max(mBasicBlockSize, int(bytes)));
        }
    }
    else {
        int64_t newSize = bytes + mTail;
        // if need a new buffer
        if (0 == mBasicBlockSize || (size_t(newSize) > mBuffers.back().capacity() && (mTail >= mBasicBlockSize || newSize >= RINGBUFFER_SINGLEBUFFER_MAXSIZE))) {
            // shrink this buffer to its current size
            mBuffers.back().resize(mTail);
            // create a new buffer
            mBuffers.push_back(std::string(std::max(mBasicBlockSize, int(bytes)), 0));
            ++mTailBuffer;
            mTail = 0;
        }
        else if (size_t(newSize) > mBuffers.back().size()) {
            mBuffers.back().resize(std::max(mBasicBlockSize, int(newSize)));
        }
    }

    char *writePtr = const_cast<char*>(mBuffers.back().data()) + mTail;
    mBufferSize += bytes;
    mTail += int(bytes);
    return writePtr;
}

char* RingBuffer::reserveFront(int64_t bytes)
{
    if (bytes <= 0 || bytes >= RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
        return NULL;
    }

    if (mHead < bytes || 0 == mBasicBlockSize) {
        if (mHead > 0) {
            mBuffers.front().erase(0, mHead);
            if (0 == mTailBuffer) {
                mTail -= mHead;
            }
        }
        mHead = std::max(int64_t(mBasicBlockSize), bytes);
        if (0 == mBufferSize) {
            if (mBuffers.empty()) {
                mBuffers.push_front(std::string(mHead, 0));
            }
            else {
                mBuffers.front().resize(mHead);
            }
            mTail = mHead;
        }
        else {
            mBuffers.push_front(std::string(mHead, 0));
            ++mTailBuffer;
        }
    }

    mHead -= int(bytes);
    mBufferSize += bytes;
    return const_cast<char*>(mBuffers.front().data()) + mHead;
}

void RingBuffer::free(int64_t bytes)
{
    MU_ASSERT2(bytes <= mBufferSize);

    while (bytes > 0) {
        int64_t blockSize = mBuffers.front().size() - mHead;
        if (0 == mTailBuffer || blockSize > bytes) {
            if (mBufferSize <= bytes) {
                // keep a single block around
                // if it doesnot exceed the basic block size
                // to avoid repeated allocations between uses of the buffer
                if (mBuffers.front().size() <= size_t(mBasicBlockSize)) {
                    mHead = 0;
                    mTail = 0;
                    mBufferSize = 0;
                }
                else {
                    // try to minify/squeeze us
                    clear();
                }
            }
            else {
                MU_ASSERT2(bytes < RINGBUFFER_SINGLEBUFFER_MAXSIZE);
                mHead += int(bytes);
                mBufferSize -= bytes;
            }
            return;
        }
        mBufferSize -= blockSize;
        bytes -= blockSize;
        mBuffers.pop_front();
        --mTailBuffer;
        mHead = 0;
    }
}

void RingBuffer::clear()
{
    if (mBuffers.empty()) {
        return;
    }

    rbuf_it it = mBuffers.begin();
    ++it;
    // keep a single block around to avoid repeated allocations
    mBuffers.erase(it, mBuffers.end());
    mBuffers.front().clear();

    mHead = 0;
    mTail = 0;
    mTailBuffer = 0;
    mBufferSize = 0;
}

int64_t RingBuffer::readLine(char *data, int64_t maxLen)
{
    if (!data || --maxLen <= 0) {
        return -1;
    }

    int64_t i = indexOf('\n', maxLen);
    i = read(data, i >= 0 ? (i + 1) : maxLen);
    // terminate it
    data[i] = '\0';
    return i;
}

int64_t RingBuffer::read(char *data, int64_t maxLen)
{
    int64_t bytesToRead = std::min(size(), maxLen);
    int64_t readSoFar = 0;
    while (readSoFar < bytesToRead) {
        int64_t bytesToReadFromThisBlock = std::min(bytesToRead - readSoFar, nextDataBlockSize());
        if (data) {
            ::memcpy(data + readSoFar, readPointer(), bytesToReadFromThisBlock);
        }
        readSoFar += bytesToReadFromThisBlock;
        free(bytesToReadFromThisBlock);
    }
    return readSoFar;
}

std::string RingBuffer::read()
{
    if (0 == mBufferSize) {
        return "";
    }

    std::string str(mBuffers.front());
    mBuffers.pop_front();
    // avoid that resizing needlessly reallocates
    str.reserve(0);
    if (0 == mTailBuffer) {
        str.resize(mTail);
        mTail = 0;
    }
    else {
        --mTailBuffer;
    }
    // does nothing if head is 0
    str.erase(0, mHead);
    mHead = 0;
    mBufferSize -= str.size();
    return str;
}

int64_t RingBuffer::peek(char *data, int64_t maxLen, int64_t pos) const
{
    int64_t readSoFar = 0;
    if (pos >= 0) {
        pos += mHead;
        rbuf_cit it = mBuffers.begin();
        for (size_t i = 0; readSoFar < maxLen && i < mBuffers.size() && it != mBuffers.end(); ++i, ++it) {
            int64_t blockLen = (i == (size_t)mTailBuffer ? mTail : it->size());
            if (pos < blockLen) {
                blockLen = std::min(blockLen - pos, maxLen - readSoFar);
                ::memcpy(data + readSoFar, it->data() + pos, blockLen);
                readSoFar += blockLen;
                pos = 0;
            }
            else {
                pos -= blockLen;
            }
        }
    }
    return readSoFar;
}

void RingBuffer::append(const char *data, int64_t size)
{
    char *writePointer = reserve(size);
    if (1 == size) {
        *writePointer = *data;
    }
    else if (size) {
        ::memcpy(writePointer, data, size);
    }
}

void RingBuffer::append(const std::string &str)
{
    if (0 == mTail) {
        if (mBuffers.empty()) {
            mBuffers.push_back(str);
        }
        else {
            mBuffers.back() = str;
        }
    }
    else {
        mBuffers.back().resize(mTail);
        mBuffers.push_back(str);
        ++mTailBuffer;
    }
    mTail = str.size();
    mBufferSize += mTail;
}

MU_END_NAMESPACE
