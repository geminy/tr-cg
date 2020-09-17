#include "Log.h"
#include "Process.h"
#include "../thread/Mutex.h"
#include "../thread/Thread.h"
#include "../tool/DateTime.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>

#define MU_LOG_BUFSZ 1024

#define MU_LOG_DEBUG_STR   "[D]"
#define MU_LOG_WARNING_STR "[W]"
#define MU_LOG_ERROR_STR   "[E]"

//#define MU_LOG_FORMATTIME

MU_USE_NAMESPACE

static Mutex gLogMutex;

//==================================================
//==================================================

void Log::Message(const char *formatStr, ...)
{
    ::va_list params;
    char buf[MU_LOG_BUFSZ] = { 0 };
    ::va_start(params, formatStr);
    ::vsnprintf(buf, MU_LOG_BUFSZ - 1, formatStr, params);
    ::va_end(params);

    gLogMutex.lock();
    ::fwrite(buf, 1, ::strlen(buf), stdout);
    ::fflush(stdout);
    gLogMutex.unlock();
}

void Log::LogUtil(LogType type, const char *tag, const char *formatStr, ...)
{
    ::va_list params;
    char buf[MU_LOG_BUFSZ] = { 0 };
    ::va_start(params, formatStr);
    ::vsnprintf(buf, MU_LOG_BUFSZ - 1, formatStr, params);
    ::va_end(params);

    std::string logs;

#ifdef MU_LOG_FORMATTIME
    DateTime dt;
    dt.makeCurrent(TIME_CLOCK_REALTIME);
    char now[MU_LOG_BUFSZ] = { 0 };
    ::sprintf(now, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            dt.date().year(),
            dt.date().month(),
            dt.date().day(),
            dt.time().hour(),
            dt.time().minute(),
            dt.time().second(),
            dt.time().msec());

    int pid = Process::CurrentPid();
    long tid = Thread::CurrentTid();
    char ids[MU_LOG_BUFSZ] = { 0 };
    ::sprintf(ids, "%d/%ld", pid, tid);
#else
    Time t;
    t.makeCurrent();
    char now[MU_LOG_BUFSZ] = { 0 };
    ::sprintf(now, "%d.%03d", t.second(), t.msec());

    int pid = Process::CurrentPid();
    long tid = Thread::CurrentTid();
    char ids[MU_LOG_BUFSZ] = { 0 };
    ::sprintf(ids, "%d/%ld", pid, tid);
#endif

    logs.append("[").append(now).append("]");
    logs.append("[").append(ids).append("]");
    if (Log_Error == type) {
        logs.append(MU_LOG_ERROR_STR);
    }
    else if (Log_Warning == type) {
        logs.append(MU_LOG_WARNING_STR);
    }
    else {
        logs.append(MU_LOG_DEBUG_STR);
    }
    logs.append("[").append(tag).append("]");
    logs += buf;

    gLogMutex.lock();
    ::fwrite(logs.data(), 1, logs.size(), stdout);
    ::fflush(stdout);
    gLogMutex.unlock();
}

//==================================================
//==================================================

Log::TempLog::TempLog(const char *msg)
    : mMsg(msg)
{
    Log::Message("********** %s begin **********\n", mMsg);
}

Log::TempLog::~TempLog()
{
    Log::Message("********** %s end **********\n", mMsg);
}

//==================================================
//==================================================

void _mu_assert_fail(const char *_expr, const char *_file, int _line, const char *_func, const char *_msg)
{
    char buf[MU_LOG_BUFSZ] = { 0 };
    ::sprintf(buf, "MU: %s failed! %s\n\tfile: %s\n\tline: %d\n\tfunction: %s\n",
            _expr, _msg, _file, _line, _func);
    Log::Message(buf);
    ::abort();
}
