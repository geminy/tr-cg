#include "Thread.h"

// <linux gun c
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <sys/syscall.h>
// linux>

MU_BEGIN_NAMESPACE

Thread::Thread(Object *parent)
    : Object(parent)
{
    setObjectName("Thread");
}

Thread::~Thread()
{

}

long Thread::CurrentTid()
{
    return ::syscall(__NR_gettid);
}

MU_END_NAMESPACE
