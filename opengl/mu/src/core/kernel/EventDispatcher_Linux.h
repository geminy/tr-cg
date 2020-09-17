/**
  * @file EventDispatcher_Linux.h
  */

#ifndef MU_EVENTDISPATCHER_LINUX_H
#define MU_EVENTDISPATCHER_LINUX_H

#include "AbstractEventDispatcher.h"

MU_BEGIN_NAMESPACE

/**
 * @class EventDispatcherLinux
 */
class MU_EXPORT EventDispatcherLinux : public AbstractEventDispatcher
{
public:
    explicit EventDispatcherLinux(Object *parent = NULL);
    ~EventDispatcherLinux();

    bool processEvents(EventLoop::EventLoopFlag flag, int maxTime);

private:
    MU_DISABLE_COPY(EventDispatcherLinux)
};

MU_END_NAMESPACE

#endif // MU_EVENTDISPATCHER_LINUX_H
