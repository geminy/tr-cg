#include "FSFileEngine.h"

#include <cstdio>

MU_BEGIN_NAMESPACE

FSFileEngine::FSFileEngine(const std::string &name)
    : mFileName(name)
{

}

FSFileEngine::~FSFileEngine()
{

}

bool FSFileEngine::flush()
{
    if (mFileName.empty()) {
        return ::fflush(stdout) == 0 ? true : false;
    }
    return false;
}

int64_t FSFileEngine::write(const char *data, int64_t len)
{
    if (mFileName.empty()) {
        return ::fwrite(data, 1, len, stdout);
    }
    return false;
}

MU_END_NAMESPACE
