/**
  * @file FileInfo.h
  */

#ifndef MU_FILEINFO_H
#define MU_FILEINFO_H

#include "../global/Global.h"

#include <string>

MU_BEGIN_NAMESPACE

/**
 * @class FileInfo
 */
class MU_EXPORT FileInfo
{
public:
    explicit FileInfo(const std::string &name);
    FileInfo(const FileInfo *other);
    FileInfo& operator=(const FileInfo &other);
    ~FileInfo();

    std::string filePath() const;
    std::string baseName() const;
    std::string dirName() const;

private:
    std::string mFileName;
};

MU_END_NAMESPACE

#endif // MU_FILEINFO_H
