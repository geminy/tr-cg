/**
  * @file FSFileEngine.h
  */

#ifndef MU_FSFILEENGINE_H
#define MU_FSFILEENGINE_H

#include "AbstractFileEngine.h"

MU_BEGIN_NAMESPACE

/**
 * @class FSFileEngine
 */
class MU_EXPORT FSFileEngine : public AbstractFileEngine
{
public:
    explicit FSFileEngine(const std::string &name);
    ~FSFileEngine();

    bool flush();
    int64_t write(const char *data, int64_t len);

private:
    MU_DISABLE_COPY(FSFileEngine)

private:
    std::string mFileName;
};

MU_END_NAMESPACE

#endif // MU_FSFILEENGINE_H
