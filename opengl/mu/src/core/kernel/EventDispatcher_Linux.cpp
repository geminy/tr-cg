#include "EventDispatcher_Linux.h"

MU_BEGIN_NAMESPACE

EventDispatcherLinux::EventDispatcherLinux(Object *parent)
    : AbstractEventDispatcher(parent)
{
    setObjectName("EventDispatcher");
}

EventDispatcherLinux::~EventDispatcherLinux()
{

}

bool EventDispatcherLinux::processEvents(EventLoop::EventLoopFlag flag, int maxTime)
{
    return true;
}

MU_END_NAMESPACE
