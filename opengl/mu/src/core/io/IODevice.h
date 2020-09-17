/**
  * @file IODevice.h
  */

#ifndef MU_IODEVICE_H
#define MU_IODEVICE_H

#include "../kernel/Object.h"

MU_BEGIN_NAMESPACE

class IODevice;
class IODevicePrivate;

/**
 * @class IODevice
 */
class MU_EXPORT IODevice : public Object
{
public:
    /**
     * @class enum
     */
    enum DeviceMode
    {
        Closed = 0x00,
        ReadOnly = 0x01,
        WriteOnly = 0x02,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 0x04,
        Truncate = 0x08,
        Text = 0x10,
        Unbuffered = 0x20
    };

public:
    /**
     * @brief constructor
     * @param parent[in] object parent
     */
    explicit IODevice(Object *parent = NULL);

    /**
     * @brief destructor
     */
    virtual ~IODevice();

    /**
     * @brief open the device and set its mode
     * @param mode[in] current device mode
     * @return true if successful, otherwise false
     */
    virtual bool open(DeviceMode mode);

    /**
     * @brief close the device and reset its mode
     */
    virtual void close();

    /**
     * @brief check device type from sequential and random-access devices
     * @return true if device is sequential, otherwise false
     * @note default false, may need reimplement
     */
    virtual bool isSequential() const;

    /**
     * @brief check if a complete line of data can be read from the buffered device
     * @return true if a complete line of data can be read, otherwise false
     */
    virtual bool canReadLine() const;

    /**
     * @brief set current positon for random-access devices
     * @param pos[in] position to set current
     * @return true on success, otherwise false
     */
    virtual bool seek(int64_t pos);

    /**
     * @brief reset current positon to the start for random-access devices
     * @return true on success, otherwise false
     */
    virtual bool reset();

    /**
     * @brief check if the current position is at the end of the device
     * @return true if the current position is at the end of the device, otherwise false
     */
    virtual bool atEnd() const;

    /**
     * @brief get the number of bytes that are available for reading from buffered sequential devices
     * @return number of bytes allocated in the buffer for reading
     */
    virtual int64_t bytesAvailable() const;

    /**
     * @brief get the number of bytes waiting to be written for buffered devices
     * @return number of bytes waiting to be written
     */
    virtual int64_t bytesToWrite() const;

    /**
     * @brief wait until data is available for reading or timeout
     * @param msecs[in] timeout by milliseconds, -1 will never timeout
     * @return true if data is available for reading, otherwise false
     * @note default false, may need reimplement
     */
    virtual bool waitForReadyRead(int msecs);

    /**
     * @brief wait until buffered written data comes or timeout and return immediately for unbuffered devices
     * @param msecs[in] timeout by milliseconds, -1 will never timeout
     * @return true if data comes, otherwise false
     * @note default false, may need reimplement
     */
    virtual bool waitForBytesWritten(int msecs);

    // todo async callbacks
    virtual void readyRead();
    virtual void channelReadyRead(int channel);
    virtual void bytesWritten(int64_t bytes);
    virtual void channelBytesWritten(int channel, int64_t bytes);
    virtual void aboutToClose();
    virtual void readChannelFinished();

    /**
     * @brief get a description of the last device error
     * @return description of the last device error
     */
    std::string errorString() const;

    /**
     * @brief get the size of the open device
     * @return size of the open device
     */
    int64_t size() const;

    /**
     * @brief get the current read/write position of random-access devices
     * @return current read/write position
     */
    int64_t pos() const;

    /**
     * @brief get the mode in which the device has been opened
     * @return the device mode
     */
    DeviceMode deviceMode() const;

    /**
     * @brief check if the device is open
     * @return true on device opened, otherwise false
     */
    bool isOpen() const;

    /**
     * @brief check if data can be read from the device
     * @return true if readable, otherwise false
     */
    bool isReadable() const;

    /**
     * @brief check if data can be written to the device
     * @return true if writabel, otherwise false
     */
    bool isWritable() const;

    /**
     * @brief check if Text mode is enabled of the device
     * @return true on Text enabled, otherwise false
     */
    bool isTextModeEnabled() const;

    /**
     * @brief set Text mode enabled or disabled
     * @param enabled[in] Text mode enabled or disabled
     */
    void setTextModeEnabled(bool enabled);

    /**
     * @brief get the number of available read channels if the device is open
     * @return number of read channels
     */
    int readChannelCount() const;

    /**
     * @brief get the number of available write channels if the device is open
     * @return number of write channels
     */
    int writeChannelCount() const;

    /**
     * @brief get the index of the current read channel
     * @return current read channel index
     */
    int currentReadChannel() const;

    /**
     * @brief set the index of the current read channel
     * @param channel[in] read channel to set current
     */
    void setCurrentReadChannel(int channel);

    /**
     * @brief get the index of the current write channel
     * @return current write channel index
     */
    int currentWriteChannel() const;

    /**
     * @brief set the index of the current write channel
     * @param channel[in] write channel to set current
     */
    void setCurrentWriteChannel(int channel);

    /**
     * @brief start a new read transaction on the device
     *
     * defines a restorable point within the sequence of read operations.
     * for sequential devices, read data will be duplicated internally
     * to allow recovery in case of incomplete reads.
     * for random-access devices, this function saves the current position.
     * call commitTransaction() or rollbackTransaction() to finish the transaction.
     */
    void startTransaction();

    /**
     * @brief complete a read transaction
     *
     * for sequential devices, all data recorded in the internal buffer
     * during the transaction will be discarded.
     */
    void commitTransaction();

    /**
     * @brief roll back a read transaction
     *
     * restore the input stream to the point of the startTransaction() call.
     * this function is commonly used to rollback the transaction when
     * an incomplete read was detected prior to committing the transaction.
     */
    void rollbackTransaction();

    /**
     * @brief check if transaction is in progress on the device
     * @return true if transaction is in progress on the device, otherwise false
     */
    bool isTransactionStarted() const;

    /**
     * @brief get one character from the device
     * @param c[out] store the character or discard the character if c is NULL
     * @return true on success, otherwise false
     */
    bool getChar(char *c);

    /**
     * @brief similar to calling getChar firstly, then putChar
     * @param c[in] character to write
     */
    void ungetChar(char c);

    /**
     * @brief put one character to the device
     * @param c[in] character to put
     * @return true on success, otherwise false
     */
    bool putChar(char c);

    /**
     * @brief read a line of ASCII characters up to a maximum of maxLen-1 bytes from the device
     * @param data[out] store the characters
     * @param maxLen[in] max size to read
     * @return number of bytes read
     */
    int64_t readLine(char *data, int64_t maxLen);

    /**
     * @brief read a line of no more than maxLen characters from the device
     * @param maxLen[in] max size to read
     * @return read line result
     */
    std::string readLine(int64_t maxLen);

    /**
     * @brief read at most maxLen bytes from the device into data
     * @param data[out] store read data
     * @param maxLen[in] max size to read
     * @return number of bytes read
     */
    int64_t read(char *data, int64_t maxLen);

    /**
     * @brief read at most maxLen bytes from the device
     * @param maxLen[in] max size to read
     * @return read data
     */
    std::string read(int64_t maxLen);

    /**
     * @brief real all remaining data from the device
     * @return read data
     */
    std::string readAll();

    /**
     * @brief write at most maxLen bytes from data to the device
     * @param data[in] data source
     * @param maxLen[in] max length to write
     * @return written data size
     */
    int64_t write(const char *data, int64_t maxLen);

    /**
     * @brief write '\0' ended data to the device
     * @param data[in] data source
     * @return written data size
     */
    int64_t write(const char *data);

    /**
     * @brief write data of str to the device
     * @param str[in] data source
     * @return written data size
     */
    int64_t write(const std::string &str);

    /**
     * @brief peek at most maxLen bytes from the device into data without side effects
     * @param data[out] peeked data
     * @param maxLen[in] max size to peek
     * @return peeked data size
     */
    int64_t peek(char *data, int64_t maxLen);

    /**
     * @brief peek at most maxLen bytes from the device without side effects
     * @param maxLen[in] max size to peek
     * @return peeked data
     */
    std::string peek(int64_t maxLen);

public:
    MU_DECLARE_PRIVATE(IODevice)

protected:
    /**
     * @brief constructor
     * @param dd[in] IODevice private data
     * @param parent[in] object parent
     */
    explicit IODevice(IODevicePrivate &dd, Object *parent = NULL);

    /**
     * @brief read up to maxLen characters from a single line into data
     * @param data[out] store data
     * @param maxLen[in] max size to read
     * @return the number of characters read
     */
    virtual int64_t readLineData(char *data, int64_t maxLen);

    /**
     * @brief read up to maxLen bytes from the device into data
     * @param data[out] store data
     * @param maxLen[in] max size to read
     * @return read data size
     * @note pure virtual function, need reimplement
     */
    virtual int64_t readData(char *data, int64_t maxLen) = 0;

    /**
     * @brief write up to maxLen bytes from data to the device
     * @param data[in] data source
     * @param maxLen[in] max size to write
     * @return written data size
     * @note pure virtual function, need reimplement
     */
    virtual int64_t writeData(const char *data, int64_t len) = 0;

    /**
     * @brief set the device mode
     * @param mode[in] device mode to set
     */
    void setDeviceMode(DeviceMode mode);

    /**
     * @brief set the description of the last device error
     * @param errorString[in] error string
     * @return void
     */
    void setErrorString(const std::string &errorString);

private:
    MU_DISABLE_COPY(IODevice)
};

MU_END_NAMESPACE

#endif // MU_IODEVICE_H
