/**
  * @file Singleton.h
  */

#ifndef MU_SINGLETON_H
#define MU_SINGLETON_H

#include "../global/Global.h"

MU_BEGIN_NAMESPACE

/**
 * @class WeakSingleton
 */
template <class T>
class MU_EXPORT WeakSingleton
{
public:
    static T* Instance() { return mSelf; }

protected:
    WeakSingleton() {  MU_ASSERT2(!mSelf); mSelf = reinterpret_cast<T*>(this); }
    ~WeakSingleton() { MU_ASSERT2(mSelf); mSelf = NULL; }

private:
    static T *mSelf;

private:
    MU_DISABLE_COPY(WeakSingleton)
};

template <class T>
T* WeakSingleton<T>::mSelf = NULL;

MU_END_NAMESPACE

#endif // MU_SINGLETON_H
