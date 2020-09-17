/**
  * @file IODevicePrivate.h
  */

#ifndef MU_IODEVICEPRIVATE_H
#define MU_IODEVICEPRIVATE_H

#include "IODevice.h"
#include "../kernel/ObjectPrivate.h"
#include "../tool/RingBuffer.h"

MU_BEGIN_NAMESPACE

#define IODEVICE_BUFSZ 16384

/**
 * @class IODevicePrivate
 */
class MU_EXPORT IODevicePrivate : public ObjectPrivate
{
public:
    /**
     * @class RingBufferRef
     */
    class RingBufferRef
    {
    public:
        bool isEmpty() const { return !mBuf || mBuf->isEmpty(); }
        int64_t size() const { return (mBuf ? mBuf->size() : int64_t(0)); }
        int chunkSize() const { MU_ASSERT2(mBuf); return mBuf->chunkSize(); }
        void setChunkSize(int size) { MU_ASSERT2(mBuf); mBuf->setChunkSize(size); }
        int64_t nextDataBlockSize() const { return (mBuf ? mBuf->nextDataBlockSize() : int64_t(0)); }
        const char *readPointer() const { return (mBuf ? mBuf->readPointer() : NULL); }
        const char *readPointerAtPosition(int64_t pos, int64_t &len) const { MU_ASSERT2(mBuf); return mBuf->readPointerAtPosition(pos, len); }
        int64_t indexOf(char c) const { return (mBuf ? mBuf->indexOf(c, mBuf->size()) : int64_t(-1)); }
        int64_t indexOf(char c, int64_t maxLen, int64_t pos = 0) const { return (mBuf ? mBuf->indexOf(c, maxLen, pos) : int64_t(-1)); }
        void truncate(int64_t pos) { MU_ASSERT2(mBuf); mBuf->truncate(pos); }
        void chop(int64_t bytes) { MU_ASSERT2(mBuf); mBuf->chop(bytes); }
        char *reserve(int64_t bytes) { MU_ASSERT2(mBuf); return mBuf->reserve(bytes); }
        char *reserveFront(int64_t bytes) { MU_ASSERT2(mBuf); return mBuf->reserveFront(bytes); }
        int64_t skip(int64_t length) { return (mBuf ? mBuf->skip(length) : int64_t(0)); }
        void free(int64_t bytes) { MU_ASSERT2(mBuf); mBuf->free(bytes); }
        void clear() { if (mBuf) mBuf->clear(); }
        int getChar() { return (mBuf ? mBuf->getChar() : -1); }
        void ungetChar(char c) { MU_ASSERT2(mBuf); mBuf->ungetChar(c); }
        void putChar(char c) { MU_ASSERT2(mBuf); mBuf->putChar(c); }
        bool canReadLine() const { return mBuf && mBuf->canReadLine(); }
        int64_t readLine(char *data, int64_t maxLen) { return (mBuf ? mBuf->readLine(data, maxLen) : int64_t(-1)); }
        int64_t read(char *data, int64_t maxLen) { return (mBuf ? mBuf->read(data, maxLen) : int64_t(0)); }
        std::string read() { return (mBuf ? mBuf->read() : std::string()); }
        int64_t peek(char *data, int64_t maxLen, int64_t pos = 0) const { return (mBuf ? mBuf->peek(data, maxLen, pos) : int64_t(0)); }
        void append(const char *data, int64_t size) { MU_ASSERT2(mBuf); mBuf->append(data, size); }
        void append(const std::string &str) { MU_ASSERT2(mBuf); mBuf->append(str); }

    private:
        RingBufferRef() : mBuf(NULL) {}
        RingBuffer *mBuf;
        friend class IODevicePrivate;
    };

    /**
     * @enum AccessMode
     */
    enum AccessMode {
        Unset,
        Sequential,
        RandomAccess
    };

public:
    /**
     * @brief constructor
     */
    IODevicePrivate();

    /**
     * @brief destructor
     */
    virtual ~IODevicePrivate();

    /**
     * @brief putCharHelper
     * @param c
     * @return
     */
    virtual bool putCharHelper(char c);

    /**
     * @brief peek at most maxLen bytes from the device into data without side effects
     * @param data[out] peeked data
     * @param maxLen[in] max size to peek
     * @return peeked data size
     */
    virtual int64_t peek(char *data, int64_t len);

    /**
     * @brief peek at most maxLen bytes from the device without side effects
     * @param maxLen[in] max size to peek
     * @return peeked data
     */
    virtual std::string peek(int64_t maxLen);

    /**
     * @brief check device type from sequential and random-access devices
     * @return true if device is sequential, otherwise false
     */
    bool isSequential() const;

    /**
     * @brief check if the buffer is empty
     * @return true if the buffer is empty, otherwise false
     */
    bool isBufferEmpty() const;

    /**
     * @brief check if all write buffers are empty
     * @return true if all write buffers are empty, otherwise false
     */
    bool allWriteBuffersEmpty() const;

    /**
     * @brief set read channel count
     * @param count[in] read channel count to set
     */
    void setReadChannelCount(int count);

    /**
     * @brief set write channel count
     * @param count[in] write chanel count to et
     */
    void setWriteChannelCount(int count);

    /**
     * @brief set current read channel
     * @param channel[in] read channel to set current
     */
    void setCurrentReadChannel(int channel);

    /**
     * @brief set current write channel
     * @param channel[in] write channel to set current
     */
    void setCurrentWriteChannel(int channel);

    /**
     * @brief read up to maxLen bytes from the device into data
     * @param data[out] store data
     * @param maxLen[in] max size to read
     * @param peek[in] if peek is true, read without side effects
     * @return read data size
     */
    int64_t read(char *data, int64_t maxLen, bool peek = false);

    /**
     * @brief set current positon for random-access devices
     * @param pos[in] position to set current
     */
    void seekBuffer(int64_t pos);

public:
    MU_DECLARE_PUBLIC(IODevice)

public:
    std::vector<RingBuffer> mReadBuffers; // multi-channel for read buffer
    std::vector<RingBuffer> mWriteBuffers; // multi-channel for write buffer
    RingBufferRef mBuffer; // normal buffer
    RingBufferRef mWriteBuffer; // for buffered device
    std::string mErrorString;
    IODevice::DeviceMode mDeviceMode;
    int64_t mPos;
    int64_t mDevicePos;
    int mReadBufferChunkSize;
    int mWriteBufferChunkSize;
    int mReadChannelCount;
    int mWriteChannelCount;
    int mCurrentReadChannel;
    int mCurrentWriteChannel;
    int64_t mTransactionPos;
    bool mTransactionStarted;
    bool mBaseReadLineDataCalled;
    mutable AccessMode mAccessMode;
};

MU_END_NAMESPACE

#endif // MU_IODEVICEPRIVATE_H
