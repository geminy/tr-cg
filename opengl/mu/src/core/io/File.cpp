#include "File.h"
#include "AbstractFileEngine.h"

MU_BEGIN_NAMESPACE

File::File(const std::string &name, Object *parent)
    : FileDevice(parent)
    , mFileName(name)
{

}

File::~File()
{

}

AbstractFileEngine* File::engine() const
{
    if (!mEngine) {
        mEngine = AbstractFileEngine::Create(mFileName);
    }
    if (!mEngine) {
        mEngine = FileDevice::engine();
    }
    return mEngine;
}

MU_END_NAMESPACE
