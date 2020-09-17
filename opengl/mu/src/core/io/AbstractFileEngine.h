/**
  * @file AbstractFileEngine.h
  */

#ifndef MU_ABSTRACTFILEENGINE_H
#define MU_ABSTRACTFILEENGINE_H

#include "../global/Global.h"

#include <string>

MU_BEGIN_NAMESPACE

/**
 * @class AbstractFileEngine
 */
class MU_EXPORT AbstractFileEngine
{
public:
    virtual ~AbstractFileEngine() = 0;

    static AbstractFileEngine* Create(const std::string &name = "");

    virtual bool flush() = 0;
    virtual int64_t write(const char *data, int64_t len) = 0;

protected:
    AbstractFileEngine();

private:
    MU_DISABLE_COPY(AbstractFileEngine)
};

MU_END_NAMESPACE

#endif // MU_ABSTRACTFILEENGINE_H
