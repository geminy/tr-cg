/**
  * @file CoreEvent.h
  */

#ifndef MU_COREEVENT_H
#define MU_COREEVENT_H

#include "../global/Global.h"

MU_BEGIN_NAMESPACE

/**
 * @class Event
 */
class MU_EXPORT Event
{
public:
    enum EventPriority
    {
        EventPrio_High = 1,
        EventPrio_Normal = 0,
        EventPrio_Low = -1
    };

    uint8_t mPosted : 1;
    uint8_t mReserved : 7;


    int type() const { return 0; }
};

MU_END_NAMESPACE

#endif // MU_COREEVENT_H
