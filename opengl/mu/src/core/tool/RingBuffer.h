/**
  * @file RingBuffer.h
  */

#ifndef MU_RINGBUFFER_H
#define MU_RINGBUFFER_H

#include "../global/Global.h"

#include <cstring>
#include <string>
#include <list>
#include <algorithm>

#define RINGBUFFER_CHUNKSIZE 4096

MU_BEGIN_NAMESPACE

// define as enum to force inlining
enum { RINGBUFFER_SINGLEBUFFER_MAXSIZE = INT32_MAX - sizeof(std::remove_pointer<std::string::pointer>::type) };

/**
 * @class RingBuffer
 */
class MU_EXPORT RingBuffer
{
public:
    /**
      * @brief constructor
      * @param growth[in]: ring buffer grows by chunk/block
      */
    explicit RingBuffer(int growth = RINGBUFFER_CHUNKSIZE);

    /**
      * @brief check if ring buffer is empty
      * @return true for empty ring buffer, otherwise false
      */
    bool isEmpty() const;

    /**
      * @brief get ring buffer size
      * @return ring buffer size
      */
    int64_t size() const;

    /**
      * @brief get current chunk size
      * @return current chunk size
      */
    int chunkSize() const;

    /**
      * @brief set chunk size
      * @param size[in] chunk size to set
      */
    void setChunkSize(int size);

    /**
      * @brief get next data block size
      * @return next data block size
      */
    int64_t nextDataBlockSize() const;

    /**
      * @brief get current ring buffer position
      * @return current ring buffer position
      */
    const char* readPointer() const;

    /**
      * @brief get required ring buffer position
      * @param pos[in] required ring buffer position
      * @param len[out] available length of required ring buffer block
      * @return required ring buffer position
      */
    const char* readPointerAtPosition(int64_t pos, int64_t &len) const;

    /**
      * @brief get index of specified char from ring buffer
      * @param c[in] char to get index
      * @return index of specified char
      */
    int64_t indexOf(char c) const;

    /**
      * @brief get index of specified char from ring buffer
      * @param c[in] char to get index
      * @param maxLen[in] max size to find index
      * @param pos[in] beginning position fo find index
      * @return index of specified char
      */
    int64_t indexOf(char c, int64_t maxLen, int64_t pos = 0) const;

    /**
      * @brief truncate ring buffer from specified pos to tail
      * @param pos[in] position to truncate
      */
    void truncate(int64_t pos);

    /**
      * @brief chop ring buffer from tail with specified size
      * @param bytes[in] size to chop
      */
    void chop(int64_t bytes);

    /**
      * @brief reserve specified bytes from ring buffer tail
      * @param bytes[in] size to reserve
      * @return old ring buffer tail
      */
    char* reserve(int64_t bytes);

    /**
      * @brief reserve specified bytes from ring buffer head
      * @param bytes[in] size to reserve
      * @return new ring buffer head
      */
    char* reserveFront(int64_t bytes);

    /**
      * @brief skip specified size from ring buffer
      * @param len[in] size to skip
      * @return skipped size
      */
    int64_t skip(int64_t len);

    /**
      * @brief free specified bytes from ring buffer head
      * @param bytes[in] size to free
      */
    void free(int64_t bytes);

    /**
      * @brief clear ring buffer
      */
    void clear();

    /**
      * @brief get char from current ring buffer position
      * @return getted char
      */
    int getChar();

    /**
      * @brief unget(putChar after getChar) char at current ring buffer position
      * @param c[in] char to unget
      */
    void ungetChar(char c);

    /**
      * @brief put char at current ring buffer position
      * @param c[in] char to put
      */
    void putChar(char c);

    /**
      * @brief check if ring buffer can be read by line
      * @return true if ring buffer can be read by line, otherwise false
      */
    bool canReadLine() const;

    /**
      * @brief read one line from ring buffer
      * @param maxLen[in] max size to read
      * @param data[out] store read line
      * @return size of read line
      */
    int64_t readLine(char *data, int64_t maxLen);

    /**
      * @brief read data from ring buffer
      * @param maxLen[in] max size to read
      * @param data[out] store read data
      * @return size of read data
      */
    int64_t read(char *data, int64_t maxLen);

    /**
      * @brief read data from first buffer
      * @return data read from first buffer
      */
    std::string read();

    /**
      * @brief read data from ring buffer
      * @param maxLen[in] max size to read
      * @param pos[in] beginning position to read
      * @param data[out] store read data
      * @return size of read data
      * @note just copy data and donot move the buffer index
      */
    int64_t peek(char *data, int64_t maxLen, int64_t pos = 0) const;

    /**
      * @brief append data to ring buffer tail
      * @param data[in] data to append
      * @param size[in] size to append
      */
    void append(const char *data, int64_t size);

    /**
      * @brief append buffer for ring buffer tail
      * @param str[in] buffer to append
      */
    void append(const std::string &str);

private:
    typedef std::list<std::string> rbuf_t;
    typedef rbuf_t::iterator rbuf_it;
    typedef rbuf_t::const_iterator rbuf_cit;

private:
    rbuf_t mBuffers; // string is a single buffer and list<string> is a ring buffer
    int mHead; // head position of first buffer
    int mTail; // tail position of last buffer
    int mTailBuffer; // last buffer
    int mBasicBlockSize; // block/chunk size
    int64_t mBufferSize; // available ring buffer size
};

//==================================================
//==================================================

inline RingBuffer::RingBuffer(int growth)
    : mHead(0)
    , mTail(0)
    , mTailBuffer(0)
    , mBasicBlockSize(growth)
    , mBufferSize(0)
{

}

inline bool RingBuffer::isEmpty() const
{
    return 0 == mBufferSize;
}

inline int64_t RingBuffer::size() const
{
    return mBufferSize;
}

inline int RingBuffer::chunkSize() const
{
    return mBasicBlockSize;
}

inline void RingBuffer::setChunkSize(int size)
{
    mBasicBlockSize = size;
}

inline int64_t RingBuffer::nextDataBlockSize() const
{
    return (0 == mTailBuffer ? mTail : mBuffers.front().size()) - mHead;
}

inline const char* RingBuffer::readPointer() const
{
    return 0 == mBufferSize ? NULL : (mBuffers.front().data() + mHead);
}

inline int64_t RingBuffer::indexOf(char c) const
{
    return indexOf(c, size());
}

inline void RingBuffer::truncate(int64_t pos)
{
    if (pos < size()) {
        chop(size() - pos);
    }
}

inline int64_t RingBuffer::skip(int64_t len)
{
    int64_t bytesToSkip = std::min(len, mBufferSize);
    free(bytesToSkip);
    return bytesToSkip;
}

inline int RingBuffer::getChar()
{
    if (isEmpty()) {
        return -1;
    }
    char c = *readPointer();
    free(1);
    return int(uint8_t(c));
}

inline void RingBuffer::ungetChar(char c)
{
    if (mHead > 0) {
        --mHead;
        mBuffers.front()[mHead] = c;
        ++mBufferSize;
    }
    else {
        char *ptr = reserveFront(1);
        *ptr = c;
    }
}

inline void RingBuffer::putChar(char c)
{
    char *ptr = reserve(1);
    *ptr = c;
}

inline bool RingBuffer::canReadLine() const
{
    return indexOf('\n') >= 0;
}

MU_END_NAMESPACE

#endif // MU_RINGBUFFER_H
