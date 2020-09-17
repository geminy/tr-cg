/**
  * @file Process.h
  */

#ifndef MU_PROCESS_H
#define MU_PROCESS_H

#include "IODevice.h"

MU_BEGIN_NAMESPACE

/**
 * @class Process
 */
class MU_EXPORT Process : public IODevice
{
public:
    explicit Process(Object *parent = NULL);
    ~Process();

    static int CurrentPid();

protected:
    int64_t writeData(const char *data, int64_t len);

private:
    MU_DISABLE_COPY(Process)
};

MU_END_NAMESPACE

#endif // MU_PROCESS_H
