/**
  * @file Thread.h
  */

#ifndef MU_THREAD_H
#define MU_THREAD_H

#include "../kernel/Object.h"

MU_BEGIN_NAMESPACE

/**
 * @class Thread
 */
class MU_EXPORT Thread : public Object
{
public:
    explicit Thread(Object *parent = NULL);
    ~Thread();

    static long CurrentTid();
    static Thread* CurrentThread() { return NULL; }

private:
    MU_DISABLE_COPY(Thread)
};

MU_END_NAMESPACE

#endif // MU_THREAD_H
