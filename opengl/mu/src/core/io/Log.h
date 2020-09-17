/**
  * @file Log.h
  */

#ifndef MU_LOG_H
#define MU_LOG_H

#include "../global/Global.h"

MU_BEGIN_NAMESPACE

/**
 * @class Log
 * @todo 1. log to file
 *       2. log filter
 *       3. Thread safety but MT-Unsafe(multi-process)
 */
class MU_EXPORT Log
{
public:
    /**
      * @enum LogType
      */
    enum LogType
    {
        Log_Debug,
        Log_Warning,
        Log_Error
    };

public:
    /**
      * @brief log a simple format message to stdout
      * @param formatStr[in] format string
      * @param ...[in] other arguments relative to formatStr
      */
    static void  Message(const char *formatStr, ...);

    /**
      * @brief log utility with log type and log tag
      * @param type[in] log type
      * @param tag[in] log tag/module
      * @param formatStr[in] format string
      * @param ...[in] other arguments relative to formatStr
      */
    static void  LogUtil(LogType type, const char *tag, const char *formatStr, ...);

public:
    /**
     * @class TempLog
     */
    class MU_EXPORT TempLog
    {
    public:
        /**
          * @brief constructor
          * @param msg[in] log message
          */
        TempLog(const char *msg = "TempLog");

        /**
          * @brief destructor
          */
        ~TempLog();

    private:
        MU_DISABLE_COPY(TempLog)

    private:
        const char *mMsg;
    };

private:
    MU_DISABLE_COPY(Log)
};

#define mlog Log::Message

#define mlogd(TAG, FORMAT, ...) Log::LogUtil(Log::Log_Debug,   TAG, FORMAT, ##__VA_ARGS__)
#define mlogw(TAG, FORMAT, ...) Log::LogUtil(Log::Log_Warning, TAG, FORMAT, ##__VA_ARGS__)
#define mloge(TAG, FORMAT, ...) Log::LogUtil(Log::Log_Error,   TAG, FORMAT, ##__VA_ARGS__)

#define mtemplog(STR) Log::TempLog _tempLog(STR)

MU_END_NAMESPACE

#endif // MU_LOG_H
