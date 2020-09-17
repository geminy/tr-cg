#include "FileDevice.h"
#include "FSFileEngine.h"

MU_BEGIN_NAMESPACE

FileDevice::FileDevice(Object *parent)
    : IODevice(parent)
    , mEngine(NULL)
{

}

FileDevice::~FileDevice()
{
    if (mEngine) {
        delete mEngine;
        mEngine = NULL;
    }
}

bool FileDevice::flush()
{
    if (!engine()) {
        return false;
    }
    return engine()->flush();
}

AbstractFileEngine* FileDevice::engine() const
{
    if (!mEngine) {
        mEngine = new FSFileEngine("");
    }
    return mEngine;
}

int64_t FileDevice::readData(char *data, int64_t len)
{
    return -1;
}

int64_t FileDevice::writeData(const char *data, int64_t len)
{
    if (!engine()) {
        return -1;
    }
    return engine()->write(data, len);
}

MU_END_NAMESPACE
