/**
  * @file FileDevice.h
  */

#ifndef MU_FILEDEVICE_H
#define MU_FILEDEVICE_H

#include "IODevice.h"

MU_BEGIN_NAMESPACE

class AbstractFileEngine;

/**
 * @class FileDevice
 */
class MU_EXPORT FileDevice : public IODevice
{
public:
    virtual ~FileDevice();

    bool flush();

protected:
    explicit FileDevice(Object *parent = NULL);

    virtual AbstractFileEngine* engine() const;

    int64_t readData(char *data, int64_t len);
    int64_t writeData(const char *data, int64_t len);

protected:
    mutable AbstractFileEngine *mEngine;

private:
    MU_DISABLE_COPY(FileDevice)
};

MU_END_NAMESPACE

#endif // MU_FILEDEVICE_H
