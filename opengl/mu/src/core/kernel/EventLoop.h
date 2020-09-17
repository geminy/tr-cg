/**
  * @file EventLoop.h
  */

#ifndef MU_EVENTLOOP_H
#define MU_EVENTLOOP_H

#include "Object.h"

MU_BEGIN_NAMESPACE

/**
 * @class EventLoop
 */
class MU_EXPORT EventLoop
{
public:
    enum EventLoopFlag
    {
        AllEvents
    };

    int exec(EventLoopFlag flag = AllEvents) { return 0; }
    void exit(int returnCode = 0) {}
};

MU_END_NAMESPACE

#endif // MU_EVENTLOOP_H
