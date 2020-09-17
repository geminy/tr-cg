/**
  * @file AbstractEventDispatcher.h
  */

#ifndef MU_ABSTRACTEVENTDISPATCHER_H
#define MU_ABSTRACTEVENTDISPATCHER_H

#include "Object.h"
#include "EventLoop.h"

MU_BEGIN_NAMESPACE

/**
 * @class AbstractEventDispatcher
 */
class MU_EXPORT AbstractEventDispatcher : public Object
{
public:
    explicit AbstractEventDispatcher(Object *parent = NULL) : Object(parent) {}
    virtual ~AbstractEventDispatcher() {}

    virtual bool processEvents(EventLoop::EventLoopFlag flag, int maxTime) = 0;

    virtual void  startUp() {}
    virtual void closeDown() {}

private:
    MU_DISABLE_COPY(AbstractEventDispatcher)
};

MU_END_NAMESPACE

#endif // MU_ABSTRACTEVENTDISPATCHER_H
