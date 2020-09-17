#include "IODevicePrivate.h"
#include "Log.h"

MU_BEGIN_NAMESPACE

IODevice::IODevice(Object *parent)
    : Object(*new IODevicePrivate, parent)
{
    setObjectName("IODevice");
}

IODevice::IODevice(IODevicePrivate &dd, Object *parent)
    : Object(dd, parent)
{
    setObjectName("IODevice");
}

IODevice::~IODevice()
{

}

bool IODevice::open(DeviceMode mode)
{
    dFunc()->mDeviceMode = mode;
    dFunc()->mPos = (mode & Append) ? size() : int64_t(0);
    dFunc()->mAccessMode = IODevicePrivate::Unset;
    dFunc()->mReadBuffers.clear();
    dFunc()->mWriteBuffers.clear();
    dFunc()->setReadChannelCount(isReadable() ? 1 : 0);
    dFunc()->setWriteChannelCount(isWritable() ? 1 : 0);
    return true;
}

void IODevice::close()
{
    if (deviceMode() == Closed) {
        return;
    }

    aboutToClose();

    dFunc()->mDeviceMode = Closed;
    dFunc()->mErrorString.clear();
    dFunc()->mPos = 0;
    dFunc()->mTransactionStarted = false;
    dFunc()->mTransactionPos = 0;
    dFunc()->setReadChannelCount(0);
    // donot setWriteChannelCount(0) that clears write buffers to allow delayed close in sockets
    dFunc()->mWriteChannelCount = 0;
}

// sequential devices, as opposed to random-access devices,
// have no concept of a start, an end, a size, or a current position,
// and they do not support seeking.
// you can only read from the device when it reports that data is available.
// the most common example of a sequential device is a network socket.
// on linux, special files such as /dev/zero and fifo pipes are sequential.
// regular files, on the other hand, do support random access.
// they have both a size and a current position, and they also support
// seeking backwards and forwards in the data stream.
// regular files are non-sequential.
bool IODevice::isSequential() const
{
    return false;
}

bool IODevice::canReadLine() const
{
    return dFunc()->mBuffer.indexOf('\n', dFunc()->isSequential() ? dFunc()->mTransactionPos : int64_t(0)) >= 0;
}

bool IODevice::seek(int64_t pos)
{
    if (dFunc()->isSequential()) {
        mlogw("MU", "IODevice::seek cannot call seek on a sequential device!\n");
        return false;
    }

    if (!isOpen()) {
        mlogw("MU", "IODevice::seek the device is not open!\n");
        return false;
    }

    if (pos < 0) {
        mlogw("MU", "IODevice::seek invalid pos: %lld\n", pos);
        return false;
    }

    dFunc()->mDevicePos = pos;
    dFunc()->seekBuffer(pos);

    return true;
}

bool IODevice::reset()
{
    return seek(0);
}

bool IODevice::atEnd() const
{
    return (!isOpen() || (dFunc()->isBufferEmpty() && bytesAvailable() == 0));
}

int64_t IODevice::bytesAvailable() const
{
    if (!dFunc()->isSequential()) {
        return std::max(size() - dFunc()->mPos, int64_t(0));
    }
    return dFunc()->mBuffer.size() - dFunc()->mTransactionPos;
}

int64_t IODevice::bytesToWrite() const
{
    return dFunc()->mWriteBuffer.size();
}

bool IODevice::waitForReadyRead(int /*msecs*/)
{
    return false;
}

bool IODevice::waitForBytesWritten(int /*msecs*/)
{
    return false;
}

void IODevice::readyRead()
{

}

void IODevice::channelReadyRead(int /*channel*/)
{

}

void IODevice::bytesWritten(int64_t /*bytes*/)
{

}

void IODevice::channelBytesWritten(int /*channel*/, int64_t /*bytes*/)
{

}

void IODevice::aboutToClose()
{

}

void IODevice::readChannelFinished()
{

}

std::string IODevice::errorString() const
{
    if (dFunc()->mErrorString.empty()) {
        return "Unknown error";
    }
    return dFunc()->mErrorString;
}

int64_t IODevice::size() const
{
    return dFunc()->isSequential() ? bytesAvailable() : int64_t(0);
}

int64_t IODevice::pos() const
{
    return dFunc()->mPos;
}

IODevice::DeviceMode IODevice::deviceMode() const
{
    return dFunc()->mDeviceMode;
}

bool IODevice::isOpen() const
{
    return deviceMode() != Closed;
}

bool IODevice::isReadable() const
{
    return (deviceMode() & ReadOnly) != 0;
}

bool IODevice::isWritable() const
{
    return (deviceMode() & WriteOnly) != 0;
}

bool IODevice::isTextModeEnabled() const
{
    return deviceMode() & Text;
}

void IODevice::setTextModeEnabled(bool enabled)
{
    if (!isOpen()) {
        mlogw("MU", "IODevice::setTextModeEnabled the device is not open!\n");
        return;
    }

    if (enabled) {
        int mode = dFunc()->mDeviceMode | Text;
        dFunc()->mDeviceMode = static_cast<IODevice::DeviceMode>(mode);
    }
    else {
        int mode = dFunc()->mDeviceMode & (~Text);
        dFunc()->mDeviceMode = static_cast<IODevice::DeviceMode>(mode);
    }
}

int IODevice::readChannelCount() const
{
    return dFunc()->mReadChannelCount;
}

int IODevice::writeChannelCount() const
{
    return dFunc()->mWriteChannelCount;
}

int IODevice::currentReadChannel() const
{
    return dFunc()->mCurrentReadChannel;
}

void IODevice::setCurrentReadChannel(int channel)
{
    if (dFunc()->mTransactionStarted) {
        mlogw("MU", "IODevice::setCurrentReadChannel failed due to read transaction is in progress!\n");
        return;
    }

    dFunc()->setCurrentReadChannel(channel);
}

int IODevice::currentWriteChannel() const
{
    return dFunc()->mCurrentWriteChannel;
}

void IODevice::setCurrentWriteChannel(int channel)
{
    dFunc()->setCurrentWriteChannel(channel);
}

void IODevice::startTransaction()
{
    if (dFunc()->mTransactionStarted) {
        mlogw("MU", "IODevice::startTransaction called while transaction already in progress!\n");
        return;
    }
    dFunc()->mTransactionPos = dFunc()->mPos;
    dFunc()->mTransactionStarted = true;
}

void IODevice::commitTransaction()
{
    if (!dFunc()->mTransactionStarted) {
        mlogw("MU", "IODevice::commitTransaction called while no transaction in progress!\n");
        return;
    }

    if (dFunc()->isSequential()) {
        dFunc()->mBuffer.free(dFunc()->mTransactionPos);
    }
    dFunc()->mTransactionStarted = false;
    dFunc()->mTransactionPos = 0;
}

void IODevice::rollbackTransaction()
{
    if (!dFunc()->mTransactionStarted) {
        mlogw("MU", "IODevice::rollbackTransaction called while no transaction in progress!\n");
        return;
    }

    if (!dFunc()->isSequential()) {
        dFunc()->seekBuffer(dFunc()->mTransactionPos);
    }
    dFunc()->mTransactionStarted = false;
    dFunc()->mTransactionPos = 0;
}

bool IODevice::isTransactionStarted() const
{
    return dFunc()->mTransactionStarted;
}

bool IODevice::getChar(char *c)
{
    char ch;
    return read(c ? c : &ch, 1) == 1;
}

void IODevice::ungetChar(char c)
{
    if (!isOpen()) {
        mlogw("MU", "IODevice::ungetChar device not open!\n");
        return;
    }
    if (!isReadable()) {
        mlogw("MU", "IODevice::ungetChar device not readable!\n");
        return;
    }
    if (dFunc()->mTransactionStarted) {
        mlogw("MU", "IODevice::ungetChar called while transaction is in progress!\n");
        return;
    }

    dFunc()->mBuffer.ungetChar(c);
    if (!dFunc()->isSequential()) {
        --dFunc()->mPos;
    }
}

bool IODevice::putChar(char c)
{
    return dFunc()->putCharHelper(c);
}

int64_t IODevice::readLine(char *data, int64_t maxLen)
{
    if (maxLen < 2) {
        mlogw("MU", "IODevice::readLine called with maxLen < 2\n");
        return int64_t(-1);
    }

    --maxLen; // leave room for a '\0'
    bool sequential = dFunc()->isSequential();
    bool keepDataInBuffer = sequential && dFunc()->mTransactionStarted;
    int64_t readSoFar = 0;

    if (keepDataInBuffer) {
        if (dFunc()->mTransactionPos < dFunc()->mBuffer.size()) {
            int64_t i = dFunc()->mBuffer.indexOf('\n', maxLen, dFunc()->mTransactionPos);
            readSoFar = dFunc()->mBuffer.peek(data, i >= 0 ? (i - dFunc()->mTransactionPos + 1) : maxLen, dFunc()->mTransactionPos);
            dFunc()->mTransactionPos += readSoFar;
            if (dFunc()->mTransactionPos == dFunc()->mBuffer.size()) {
                readData(data, 0);
            }
        }
        else if (!dFunc()->mBuffer.isEmpty()) {
            readSoFar = dFunc()->mBuffer.readLine(data, maxLen + 1); // RingBuffer::readLine terminates the line with '\0'
            if (dFunc()->mBuffer.isEmpty()) {
                readData(data, 0);
            }
            if (!sequential) {
                dFunc()->mPos += readSoFar;
            }
        }
    }

    if (readSoFar) {
        if (data[readSoFar -1] == '\n') {
            if (isTextModeEnabled()) {
                if (readSoFar > 1 && data[readSoFar -2] == '\r') { // RingBuffer::readLine isn't Text aware
                    --readSoFar;
                    data[readSoFar - 1] = '\n';
                }
            }
            data[readSoFar] = '\0';
            return readSoFar;
        }
    }

    if (dFunc()->mPos != dFunc()->mDevicePos && !sequential && !seek(dFunc()->mPos)) {
        return int64_t(-1);
    }

    dFunc()->mBaseReadLineDataCalled = false;
    // force base implementation for transaction on sequential device
    // as it stores the data in internal buffer automatically
    int64_t readBytes = keepDataInBuffer ? IODevice::readLineData(data + readSoFar, maxLen - readSoFar)
                                         : readLineData(data + readSoFar, maxLen - readSoFar);
    if (readBytes < 0) {
        data[readSoFar] = '\0';
        return readSoFar ? readSoFar : -1;
    }

    readSoFar += readBytes;
    if (!dFunc()->mBaseReadLineDataCalled && !sequential) {
        dFunc()->mPos += readBytes;
        // if the base implementation was not called
        // we must assume the device position is invalid and force a seek
        dFunc()->mDevicePos = int64_t(-1);
    }
    data[readSoFar] = '\0';

    if (isTextModeEnabled()) {
        if (readSoFar > 1 && data[readSoFar - 1] == '\n' && data[readSoFar - 2] == '\r') {
            data[readSoFar - 2] = '\n';
            data[readSoFar - 1] = '\0';
            --readSoFar;
        }
    }

    return readSoFar;
}

std::string IODevice::readLine(int64_t maxLen)
{
    std::string result;

    if (maxLen < 0) {
        mlogw("MU", "IODevice::readLine called with maxLen < 0\n");
        return result;
    }
    if (maxLen > RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
        mlogw("MU", "IODevice::readLine maxLen argument exceeds buffer size limit!\n");
        maxLen = RINGBUFFER_SINGLEBUFFER_MAXSIZE - 1;
    }

    result.resize(int(maxLen));
    int64_t readBytes = 0;
    if (!result.size()) {
        // if resize fails or maxLen==0, read incrementally
        // the first iteration needs to leave an extra byte for the terminating null
        if (0 == maxLen) {
            maxLen = RINGBUFFER_SINGLEBUFFER_MAXSIZE - 1;
        }
        result.resize(1);
        int64_t readResult;
        do {
            result.resize(int(std::min(maxLen, int64_t(result.size() + dFunc()->mReadBufferChunkSize))));
            readResult = readLine(const_cast<char*>(result.data()) + readBytes, result.size() - readBytes);
            if (readResult > 0 || readBytes == 0) {
                readBytes += readResult;
            }
        }
        while (readResult == dFunc()->mReadBufferChunkSize && result[int(readBytes  -1)] != '\n');
    }
    else {
        readBytes = readLine(const_cast<char*>(result.data()), result.size());
    }

    if (readBytes <= 0) {
        result.clear();
    }
    else {
        result.resize(readBytes);
    }

    return result;
}

int64_t IODevice::read(char *data, int64_t maxLen)
{
    bool sequential = dFunc()->isSequential();
    // shortcut for getChar(), unless we need to keep the data in the buffer
    if (1 == maxLen && !(sequential && dFunc()->mTransactionStarted)) {
        int chint;
        while ((chint = dFunc()->mBuffer.getChar()) != -1) {
            if (!sequential) {
                ++dFunc()->mPos;
            }
            char c = char(uint8_t(chint));
            if ('\r' == c && isTextModeEnabled()) {
                continue;
            }
            *data = c;
            if (dFunc()->mBuffer.isEmpty()) {
                readData(data, 0);
            }
            return int64_t(1);
        }
    }

    if (maxLen < 0) {
        mlogw("MU", "IODevice::read called with maxLen < 0\n");
        return -1;
    }
    if (!isOpen()) {
        mlogw("MU", "IODevice::read device not open!\n");
        return -1;
    }
    if (!isReadable()) {
        mlogw("MU", "IODevice::read device not readable!\n");
        return -1;
    }

    int64_t readBytes = dFunc()->read(data, maxLen);
    return readBytes;
}

std::string IODevice::read(int64_t maxLen)
{
    std::string result;

    // try to prevent the data from being copied
    // if we have a chunk with the same size in the read buffer
    if (maxLen == dFunc()->mBuffer.nextDataBlockSize() && !dFunc()->mTransactionStarted
            && ((dFunc()->mDeviceMode & (IODevice::ReadOnly | IODevice::Text)) == IODevice::ReadOnly)) {
        result = dFunc()->mBuffer.read();
        if (!dFunc()->isSequential()) {
            dFunc()->mPos += maxLen;
        }
        if (dFunc()->mBuffer.isEmpty()) {
            readData(NULL, 0);
        }
        return result;
    }

    if (maxLen < 0) {
        mlogw("MU", "IODevice::read called with maxLen < 0\n");
        return result;
    }
    if (maxLen > RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
        mlogw("MU", "IODevice::read maxLen argument exceeds buffer size limit!\n");
        maxLen = RINGBUFFER_SINGLEBUFFER_MAXSIZE - 1;
    }

    result.resize(int(maxLen));
    int64_t readBytes = read(const_cast<char*>(result.data()), result.size());
    if (readBytes <= 0) {
        result.clear();
    }
    else {
        result.resize(int(readBytes));
    }

    return result;
}

std::string IODevice::readAll()
{
    std::string result;
    int64_t readBytes = (dFunc()->isSequential() ? int64_t(0) : size());
    if (0 == readBytes) {
        int64_t readChunkSize = std::max(int64_t(dFunc()->mReadBufferChunkSize),
                                         dFunc()->isSequential() ? (dFunc()->mBuffer.size() - dFunc()->mTransactionPos) : dFunc()->mBuffer.size());
        int64_t readResult;
        do {
            if (readBytes + readChunkSize >= RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
                break;
            }
            result.resize(readBytes + readChunkSize);
            readResult = read(const_cast<char*>(result.data()) + readBytes, readChunkSize);
            if (readResult > 0 || readBytes == 0) {
                readBytes += readResult;
                readChunkSize = dFunc()->mReadBufferChunkSize;
            }
        }
        while (readResult > 0);
    }
    else {
        readBytes -= dFunc()->mPos;
        if (readBytes >= RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
            return "";
        }
        result.resize(readBytes);
        readBytes = read(const_cast<char*>(result.data()), readBytes);
    }

    if (readBytes <= 0) {
        result.clear();
    }
    else {
        result.resize(int(readBytes));
    }

    return result;
}

int64_t IODevice::write(const char *data, int64_t maxLen)
{
    if (!isOpen()) {
        mlogw("MU", "IODevice::write device not open!\n");
        return -1;
    }
    if (!isWritable()) {
        mlogw("MU", "IODevice::write device not writable!\n");
        return -1;
    }
    if (maxLen < 0) {
        mlogw("MU", "IODevice::write called with maxLen < 0\n");
        return -1;
    }

    bool sequential = dFunc()->isSequential();
    if (dFunc()->mPos != dFunc()->mDevicePos && !sequential && !seek(dFunc()->mPos)) {
        return int64_t(-1);
    }

    int64_t written = writeData(data, maxLen);
    if (!sequential && written > 0) {
        dFunc()->mPos += written;
        dFunc()->mDevicePos += written;
        dFunc()->mBuffer.skip(written);
    }
    return written;
}

int64_t IODevice::write(const char *data)
{
    return write(data, ::strlen(data));
}

int64_t IODevice::write(const std::string &str)
{
    return write(str.data(), str.size());
}

int64_t IODevice::peek(char *data, int64_t maxLen)
{
    if (maxLen < 0) {
        mlogw("MU", "IODevice::peek called with maxLen < 0\n");
        return -1;
    }
    if (!isOpen()) {
        mlogw("MU", "IODevice::peek device not open!\n");
        return -1;
    }
    if (!isReadable()) {
        mlogw("MU", "IODevice::peek device not readable!\n");
        return -1;
    }

    return dFunc()->peek(data, maxLen);
}

std::string IODevice::peek(int64_t maxLen)
{
    if (maxLen < 0) {
        mlogw("MU", "IODevice::peek called with maxLen < 0\n");
        return "";
    }
    if (maxLen > RINGBUFFER_SINGLEBUFFER_MAXSIZE) {
        mlogw("MU", "IODevice::peek maxLen argument exceeds buffer size limit\n");
        maxLen = RINGBUFFER_SINGLEBUFFER_MAXSIZE -1;
    }
    if (!isOpen()) {
        mlogw("MU", "IODevice::peek device not open!\n");
        return "";
    }
    if (!isReadable()) {
        mlogw("MU", "IODevice::peek device not readable!\n");
        return "";
    }

    return dFunc()->peek(maxLen);
}

int64_t IODevice::readLineData(char *data, int64_t maxLen)
{
    int64_t readSoFar = 0;
    char c;
    int lastReadReturn = 0;
    dFunc()->mBaseReadLineDataCalled = true;
    while (readSoFar < maxLen && (lastReadReturn = read(&c, 1)) == 1) {
        *data++ = c;
        ++readSoFar;
        if (c == '\n') {
            break;
        }
    }
    if (lastReadReturn != 1 && 0 == readSoFar) {
        return isSequential() ? lastReadReturn : -1;
    }
    return readSoFar;
}

void IODevice::setDeviceMode(DeviceMode mode)
{
    dFunc()->mDeviceMode = mode;
    dFunc()->mAccessMode = IODevicePrivate::Unset;
    dFunc()->setReadChannelCount(isReadable() ? std::max(dFunc()->mReadChannelCount, 1) : 0);
    dFunc()->setWriteChannelCount(isWritable() ? std::max(dFunc()->mWriteChannelCount, 1) : 0);
}

void IODevice::setErrorString(const std::string &errorString)
{
    dFunc()->mErrorString = errorString;
}

//==================================================
//==================================================

IODevicePrivate::IODevicePrivate()
    : mDeviceMode(IODevice::Closed)
    , mPos(0)
    , mDevicePos(0)
    , mReadBufferChunkSize(IODEVICE_BUFSZ)
    , mWriteBufferChunkSize(0)
    , mReadChannelCount(0)
    , mWriteChannelCount(0)
    , mCurrentReadChannel(0)
    , mCurrentWriteChannel(0)
    , mTransactionPos(0)
    , mTransactionStarted(false)
    , mBaseReadLineDataCalled(false)
    , mAccessMode(Unset)
{

}

IODevicePrivate::~IODevicePrivate()
{

}

bool IODevicePrivate::putCharHelper(char c)
{
    return mFunc()->write(&c, 1) == 1;
}

int64_t IODevicePrivate::peek(char *data, int64_t maxLen)
{
    return read(data, maxLen, true);
}

std::string IODevicePrivate::peek(int64_t maxLen)
{
    std::string str(maxLen, 0);
    int64_t readBytes = read(const_cast<char*>(str.data()), maxLen, true);
    if (readBytes < maxLen) {
        if (readBytes <= 0) {
            str.clear();
        }
        else {
            str.resize(readBytes);
        }
    }

    return str;
}

// random-access devices support seeking to arbitrary positions using seek().
// the current position in the file is available by calling pos().
// File and Buffer are examples of random-access devices.
// sequential devices don't support seeking to arbitrary positions.
// the data must be read in one pass.
// the functions pos() and size() don't work for sequential devices.
// TcpSocket and Process are examples of sequential devices.
bool IODevicePrivate::isSequential() const
{
    if (Unset == mAccessMode) {
        mAccessMode = mFunc()->isSequential() ? Sequential : RandomAccess;
    }
    return Sequential == mAccessMode;
}

bool IODevicePrivate::isBufferEmpty() const
{
    return mBuffer.isEmpty() || (mTransactionStarted && isSequential() && mTransactionPos == mBuffer.size());
}

bool IODevicePrivate::allWriteBuffersEmpty() const
{
    for (size_t i = 0; i < mWriteBuffers.size(); ++i) {
        if (!mWriteBuffers.at(i).isEmpty()) {
            return false;
        }
    }

    return true;
}

void IODevicePrivate::setReadChannelCount(int count)
{
    if (size_t(count) > mReadBuffers.size()) {
        mReadBuffers.insert(mReadBuffers.end(), count - mReadBuffers.size(), RingBuffer(mReadBufferChunkSize));
    }
    else {
        mReadBuffers.resize(count);
    }
    mReadChannelCount = count;
    setCurrentReadChannel(mCurrentReadChannel);
}

void IODevicePrivate::setWriteChannelCount(int count)
{
    if (size_t(count) > mWriteBuffers.size()) {
        if (mWriteBufferChunkSize != 0) {
            mWriteBuffers.insert(mWriteBuffers.end(), count - mWriteBuffers.size(), RingBuffer(mWriteBufferChunkSize));
        }
        else {
            // mWriteBufferChunkSize is default zero
            // donot use IODevice's write buffers
        }
    }
    else {
        mWriteBuffers.resize(count);
    }
    mWriteChannelCount = count;
    setCurrentWriteChannel(mCurrentWriteChannel);
}

void IODevicePrivate::setCurrentReadChannel(int channel)
{
    mBuffer.mBuf = (size_t(channel) < mReadBuffers.size() ? &mReadBuffers.at(channel) : NULL);
    mCurrentReadChannel = channel;
}

void IODevicePrivate::setCurrentWriteChannel(int channel)
{
    mWriteBuffer.mBuf = (size_t(channel) < mWriteBuffers.size() ? &mWriteBuffers.at(channel) : NULL);
    mCurrentWriteChannel = channel;
}

int64_t IODevicePrivate::read(char *data, int64_t maxLen, bool peek)
{
    bool buffered = (mDeviceMode & IODevice::Unbuffered) == 0;
    bool sequential = isSequential();
    bool keepDataInBuffer = sequential ? (peek || mTransactionStarted) : (peek && buffered);
    int64_t savePos = mPos;
    int64_t readSoFar = 0;
    bool madeBufferReadOnly = true;
    bool deviceAtEof = false;
    char *readPtr  = data;
    int64_t bufferPos = (sequential && mTransactionStarted) ? mTransactionPos : int64_t(0);

    while (1) {
        // try reading from the buffer
        int64_t bufferReadChunkSize = keepDataInBuffer ? mBuffer.peek(data, maxLen, bufferPos) : mBuffer.read(data, maxLen);
        if (bufferReadChunkSize > 0) {
            bufferPos += bufferReadChunkSize;
            if (!sequential) {
                mPos += bufferReadChunkSize;
            }
            readSoFar += bufferReadChunkSize;
            data += bufferReadChunkSize;
            maxLen -= bufferReadChunkSize;
        }

        if (maxLen > 0 && !deviceAtEof) {
            int64_t readFromDevice = 0;
            // make sure the device is positiond correctly
            if (sequential || mPos == mDevicePos ||  mFunc()->seek(mPos)) {
                madeBufferReadOnly = false; // fix readData attempt
                if ((!buffered || maxLen >= mReadBufferChunkSize) && !keepDataInBuffer) {
                    // read a big  chunk directly to output buffer
                    readFromDevice = mFunc()->readData(data, maxLen);
                    deviceAtEof = (readFromDevice != maxLen);
                    if (readFromDevice > 0) {
                        readSoFar += readFromDevice;
                        data += readFromDevice;
                        maxLen -= readFromDevice;
                        if (!sequential) {
                            mPos += readFromDevice;
                            mDevicePos += readFromDevice;
                        }
                    }
                }
                else {
                    // do not read more than maxLen on unbuffered devices
                    int64_t bytesToBuffer = (buffered || mReadBufferChunkSize < maxLen)
                            ? int64_t(mReadBufferChunkSize)
                            : maxLen;
                    // try to fill IODevice buffer by single read
                    readFromDevice = mFunc()->readData(mBuffer.reserve(bytesToBuffer), bytesToBuffer);
                    deviceAtEof = (readFromDevice != bytesToBuffer);
                    mBuffer.chop(bytesToBuffer - std::max(int64_t(0), readFromDevice));
                    if (readFromDevice > 0) {
                        if (!sequential) {
                            mDevicePos += readFromDevice;
                        }
                        continue;
                    }
                }
            }
            else {
                readFromDevice = -1;
            }

            if (readFromDevice < 0 && readSoFar == 0) {
                // error and we haven't read anything: return immediately
                return int64_t(-1);
            }
        }

        if ((mDeviceMode & IODevice::Text) && readPtr < data) {
            const char *endPtr = data;
            // optimization to avoid initial self-assignment
            while (*readPtr != '\r') {
                if (++readPtr == endPtr) {
                    break;
                }
            }
            char *writePtr = readPtr;
            while (readPtr < endPtr) {
                char ch = *readPtr++;
                if (ch != '\r') {
                    *writePtr++ = ch;
                }
                else {
                    --readSoFar;
                    --data;
                    ++maxLen;
                }
            }
            // make sure we get more data if there is room for more
            // this is very important for when someone seeks to the part
            // of a '\r\n' and reads one character - they should get the '\n'
            readPtr = data;
            continue;
        }

        break;
    }

    // restore positions after reading
    if (keepDataInBuffer) {
        if (peek) {
            mPos = savePos; // does nothing on sequential devices
        }
        else {
            mTransactionPos = bufferPos;
        }
    }
    else if (peek) {
        seekBuffer(savePos); // unbuffered random-access device
    }

    if (madeBufferReadOnly && isBufferEmpty()) {
        mFunc()->readData(data, 0);
    }

    return readSoFar;
}

void IODevicePrivate::seekBuffer(int64_t pos)
{
    int64_t offset = pos - mPos;
    mPos = pos;
    if (offset < 0 || offset >= mBuffer.size()) {
        mBuffer.clear();
    }
    else {
        mBuffer.free(offset);
    }
}

MU_END_NAMESPACE
