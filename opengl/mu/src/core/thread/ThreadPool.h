/**
  * @file ThreadPool.h
  */

#ifndef MU_THREADPOOL_H
#define MU_THREADPOOL_H

#include "../kernel/Object.h"

MU_BEGIN_NAMESPACE

/**
 * @class ThreadPool
 */
class MU_EXPORT ThreadPool : public Object
{
public:
    explicit ThreadPool(Object *parent = NULL);
    ~ThreadPool();

    static ThreadPool* GlobalInstance() { return NULL; }

    bool waitForDone() { return true; }

private:
    MU_DISABLE_COPY(ThreadPool)
};

MU_END_NAMESPACE

#endif // MU_THREADPOOL_H
