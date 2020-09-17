/**
  * @file File.h
  */

#ifndef MU_FILE_H
#define MU_FILE_H

#include "FileDevice.h"

MU_BEGIN_NAMESPACE

/**
 * @class File
 */
class MU_EXPORT File : public FileDevice
{
public:
    explicit File(const std::string &name, Object *parent = NULL);
    ~File();

protected:
    AbstractFileEngine* engine() const;

private:
    MU_DISABLE_COPY(File)

private:
    std::string mFileName;
};

MU_END_NAMESPACE

#endif // MU_FILE_H
