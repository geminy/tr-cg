#include "Process.h"

// <linux gun c
#include <sys/types.h>
#include <unistd.h>
// linux>

MU_BEGIN_NAMESPACE

Process::Process(Object *parent)
    : IODevice(parent)
{
    setObjectName("Process");
}

Process::~Process()
{

}

int Process::CurrentPid()
{
    return ::getpid();
}

int64_t Process::writeData(const char */*data*/, int64_t /*len*/)
{
    return -1;
}

MU_END_NAMESPACE
