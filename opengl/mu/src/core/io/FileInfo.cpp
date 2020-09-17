#include "FileInfo.h"

// <linux gnu c
#include <libgen.h>
// linux>

MU_BEGIN_NAMESPACE

FileInfo::FileInfo(const std::string &name)
    : mFileName(name)
{

}

FileInfo::FileInfo(const FileInfo *other)
    : mFileName(other->mFileName)
{

}

FileInfo& FileInfo::operator=(const FileInfo &other)
{
    mFileName = other.mFileName;
    return *this;
}

FileInfo::~FileInfo()
{

}

std::string FileInfo::filePath() const
{
    return mFileName;
}

std::string FileInfo::baseName() const
{
    return basename(const_cast<char*>(mFileName.c_str()));
}

std::string FileInfo::dirName() const
{
    return dirname(const_cast<char*>(mFileName.c_str()));
}

MU_END_NAMESPACE
