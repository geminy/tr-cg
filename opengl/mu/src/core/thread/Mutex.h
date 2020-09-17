/**
  * @file Mutex.h
  */

#ifndef MU_MUTEX_H
#define MU_MUTEX_H

#include "../global/Global.h"

// <linux gnu c
#include "pthread.h"
// linux>

MU_BEGIN_NAMESPACE

/**
 * @class Mutex
 */
class MU_EXPORT Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

private:
    MU_DISABLE_COPY(Mutex)

private:
    pthread_mutex_t mMutex;
};

/**
 * @class AutoMutex
 */
class MU_EXPORT AutoMutex
{
public:
    AutoMutex(Mutex &mutex)
        : mMutex(mutex)
    {
        mMutex.lock();
    }

    ~AutoMutex()
    {
        mMutex.unlock();
    }

private:
    MU_DISABLE_COPY(AutoMutex)

private:
    Mutex &mMutex;
};

MU_END_NAMESPACE

#endif // MU_MUTEX_H
