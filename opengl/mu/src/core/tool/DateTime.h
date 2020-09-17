/**
  * @file DateTime.h
  */

#ifndef MU_DATETIME_H
#define MU_DATETIME_H

#include "../global/Global.h"

#include <string>

MU_BEGIN_NAMESPACE

/**
 * @enum TimeType
 */
enum TimeType
{
    TIME_CLOCK_BEGIN,
    TIME_CLOCK_REALTIME,
    TIME_CLOCK_REALTIME_COARSE,
    TIME_CLOCK_MONOTONIC,
    TIME_CLOCK_MONOTONIC_COARSE,
    TIME_CLOCK_MONOTONIC_RAW,
    TIME_CLOCK_BOOTTIME,
    TIME_CLOCK_PROCESS_CPUTIME_ID,
    TIME_CLOCK_THREAD_CPUTIME_ID,
    TIME_CLOCK_END
};

/**
 * @enum TimeValue
 */
enum TimeValue
{
    SECS_PER_MIN = 60,
    SECS_PER_HOUR = 3600,
    SECS_PER_DAY = 86400,
    MSECS_PER_MIN = 60000,
    MSECS_PER_HOUR = 3600000,
    MSECS_PER_DAY = 86400000,
    TIME_T_MAX = 2145916799, // int max 2037-12-31 23:59:59 UTC
    JULIAN_DAY_FOR_EPOCH = 2440588 // julian day from date(1970, 1, 1)
};

/**
 * @class Date
 * @todo julian
 */
class MU_EXPORT Date
{
public:
    Date();
    Date(int y, int m, int d);
    Date(const Date &date);
    Date& operator=(const Date &date);
    ~Date();

    static Date CurrentDate();
    static bool IsLeapYear(int year);

    bool makeCurrent(TimeType type = TIME_CLOCK_MONOTONIC);

    bool setDate(int y, int m, int d);

    bool fromString(const std::string &date);
    std::string toString() const;

    bool isNull() const;
    bool isValid() const;

    int year() const;
    int month() const;
    int day() const;

private:
    enum
    {
        NullDate = -1
    };

private:
    bool isValid(int y, int m, int d) const;

private:
    int mYear;
    int mMonth;
    int mDay;
};

/**
 * @class Time
 */
class MU_EXPORT Time
{
public:
    Time();
    Time(int h, int m, int s, int ms);
    Time(const Time &time);
    Time& operator=(const Time &time);
    ~Time();

    static Time CurrentTime();

    bool makeCurrent(TimeType type = TIME_CLOCK_MONOTONIC);

    bool setTime(int h, int m, int s, int ms);

    bool fromString(const std::string &time);
    std::string toString() const;

    bool isNull() const;
    bool isValid() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;

    void start();
    int elapsed();

private:
    enum
    {
        NullTime = -1
    };

private:
    bool isValid(int h, int m, int s, int ms) const;
    int msecs() const;

private:
    int mMsecs; // total milliseconds in one day
};

/**
 * @class DateTime
 * @todo DateTime
 */
class MU_EXPORT DateTime
{
public:
    DateTime();
    DateTime(const Date &date, const Time &time);
    DateTime(const DateTime &dateTime);
    DateTime& operator=(const DateTime &dateTime);
    ~DateTime();

    static DateTime CurrentDateTime();

    void makeCurrent(TimeType type = TIME_CLOCK_MONOTONIC);

    void setDate(const Date &date);
    void setTime(const Time &time);
    void setDateTime(const Date &date, const Time &time);

    bool fromString(const std::string &dateTime);
    std::string toString() const;

    bool isNull() const;
    bool isValid() const;

    Date date() const;
    Time time() const;

private:
    Date mDate;
    Time mTime;
};

MU_END_NAMESPACE

#endif // MU_DATETIME_H
