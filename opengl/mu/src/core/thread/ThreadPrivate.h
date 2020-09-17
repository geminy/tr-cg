/**
  * @file ThreadPrivate.h
  */

#ifndef MU_THREADPRIVATE_H
#define MU_THREADPRIVATE_H

#include "../global/Global.h"
#include "../thread/Mutex.h"

#include <vector>

MU_BEGIN_NAMESPACE

class Object;
class Event;
class EventLoop;
class Thread;
class AbstractEventDispatcher;

/**
 * @class PostEvent
 */
class MU_EXPORT PostEvent
{
public:
    Object *mReceiver;
    Event *mEvent;
    int mPriority;
};

/**
 * @class PostEventList
 */
class MU_EXPORT PostEventList : public std::vector<PostEvent>
{
public:
    Mutex mMutex;
};

/**
 * @class ThreadData
 */
class MU_EXPORT ThreadData
{
public:
    ThreadData() {}
    ~ThreadData() {}

    static ThreadData* Current() { return NULL; }

    Thread *mThread;
    std::vector<EventLoop*> mEventLoops;
    PostEventList mPostEventList;
    AbstractEventDispatcher *mEventDispatcher;
};

MU_END_NAMESPACE

#endif // MU_THREADPRIVATE_H
