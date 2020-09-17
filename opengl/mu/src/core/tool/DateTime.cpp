#include "DateTime.h"

#include <ctime>
#include <cstdio>

MU_BEGIN_NAMESPACE

Date::Date()
    : mYear(NullDate)
    , mMonth(NullDate)
    , mDay(NullDate)
{

}

Date::Date(int y, int m, int d)
    : mYear(NullDate)
    , mMonth(NullDate)
    , mDay(NullDate)
{
    setDate(y, m, d);
}

Date::Date(const Date &date)
    : mYear(date.mYear)
    , mMonth(date.mMonth)
    , mDay(date.mDay)
{

}

Date& Date::operator=(const Date &date)
{
    mYear = date.mYear;
    mMonth = date.mMonth;
    mDay = date.mDay;
    return *this;
}

Date::~Date()
{

}

Date Date::CurrentDate()
{
    Date date;
    date.makeCurrent(TIME_CLOCK_REALTIME);
    return date;
}

bool Date::IsLeapYear(int year)
{
    if (year > 1) {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }
    return false;
}

bool Date::makeCurrent(TimeType type)
{
    if (type > TIME_CLOCK_BEGIN && type < TIME_CLOCK_END) {
        clockid_t id;
        timespec ts;
        switch (type) {
        case TIME_CLOCK_REALTIME:
            id = CLOCK_REALTIME;
            break;
        case TIME_CLOCK_REALTIME_COARSE:
            id = CLOCK_REALTIME_COARSE;
            break;
        case TIME_CLOCK_MONOTONIC:
            id = CLOCK_MONOTONIC;
            break;
        case TIME_CLOCK_MONOTONIC_COARSE:
            id = CLOCK_MONOTONIC_COARSE;
            break;
        case TIME_CLOCK_MONOTONIC_RAW:
            id = CLOCK_MONOTONIC_RAW;
            break;
        case TIME_CLOCK_BOOTTIME:
            id = CLOCK_BOOTTIME;
            break;
        case TIME_CLOCK_PROCESS_CPUTIME_ID:
            id = CLOCK_PROCESS_CPUTIME_ID;
            break;
        case TIME_CLOCK_THREAD_CPUTIME_ID:
            id = CLOCK_THREAD_CPUTIME_ID;
            break;
        default:
            id = CLOCK_REALTIME;
            break;
        }
        bool result = ::clock_gettime(id, &ts) == 0 ? true : false;
        if (result) {
            tm formatTm;
            if (::localtime_r(&ts.tv_sec, &formatTm)) {
                mYear = formatTm.tm_year + 1900;
                mMonth = formatTm.tm_mon + 1;
                mDay = formatTm.tm_mday;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return false;
}

bool Date::setDate(int y, int m, int d)
{
    if (isValid(y, m, d)) {
        mYear = y;
        mMonth = m;
        mDay = d;
        return true;
    }
    mYear = NullDate;
    mMonth = NullDate;
    mDay = NullDate;
    return false;
}

// 1970-01-01
bool Date::fromString(const std::string &date)
{
    if (date.length() != 10) {
        return false;
    }

    int c0 = date.at(0) - '0';
    int c1 = date.at(1) - '0';
    int c2 = date.at(2) - '0';
    int c3 = date.at(3) - '0';
    char c4 = date.at(4);
    int c5 = date.at(5) - '0';
    int c6 = date.at(6) - '0';
    char c7 = date.at(7);
    int c8 = date.at(8) - '0';
    int c9 = date.at(9) - '0';

    if (c4 != '-' || c7 != '-') {
        return false;
    }

    if (c0 < 0 || c0 > 9
        || c1 < 0 || c1 > 9
        || c2 < 0 || c2 > 9
        || c3 < 0 || c3 > 9
        || c5 < 0 || c5 > 9
        || c6 < 0 || c6 > 9
        || c8 < 0 || c8 > 9
        || c9 < 0 || c9 > 9) {
        return false;
    }

    int year = c0 * 1000 + c1 * 100 + c2 * 10 + c3;
    int month = c4 * 10 + c5;
    int day = c8 * 10 + c9;

    if (isValid(year, month, day)) {
        mYear = year;
        mMonth = month;
        mDay = day;
        return true;
    }

    mYear = NullDate;
    mMonth = NullDate;
    mDay = NullDate;
    return false;
}

std::string Date::toString() const
{
    char buf[10] = { 0 };
    ::snprintf(buf, 10, "%04d-%02d-%02d", mYear, mMonth, mDay);
    return std::string(buf);
}

bool Date::isNull() const
{
    if (mYear != NullDate && mMonth != NullDate && mDay != NullDate) {
        return false;
    }
    return true;
}

bool Date::isValid() const
{
    return isValid(mYear, mMonth, mDay);
}

int Date::year() const
{
    return mYear;
}

int Date::month() const
{
    return mMonth;
}

int Date::day() const
{
    return mDay;
}

bool Date::isValid(int y, int m, int d) const
{
    if (y >=1 && y <= 9999
        && m >= 1 && m <= 12
        && (((1 == m || 3 == m || 5 == m || 7 == m || 8 == m || 10 == m || 12 == m) && (31 == d))
            || ((4 == m || 6 == m || 9 == m || 11 == m) && (30 == d))
            || (IsLeapYear(y) && 29 == d)
            || (28 == d)
            )) {
        return true;
    }
    return false;
}

//==================================================
//==================================================

Time::Time()
    : mMsecs(NullTime)
{

}

Time::Time(int h, int m, int s, int ms)
    : mMsecs(NullTime)
{
    setTime(h, m, s, ms);
}

Time::Time(const Time &time)
    : mMsecs(time.mMsecs)
{

}

Time& Time::operator=(const Time &time)
{
    mMsecs = time.mMsecs;
    return *this;
}

Time::~Time()
{

}

Time Time::CurrentTime()
{
    Time time;
    time.makeCurrent(TIME_CLOCK_REALTIME);
    return time;
}

bool Time::makeCurrent(TimeType type)
{
    if (type > TIME_CLOCK_BEGIN && type < TIME_CLOCK_END) {
        clockid_t id;
        timespec ts;
        switch (type) {
        case TIME_CLOCK_REALTIME:
            id = CLOCK_REALTIME;
            break;
        case TIME_CLOCK_REALTIME_COARSE:
            id = CLOCK_REALTIME_COARSE;
            break;
        case TIME_CLOCK_MONOTONIC:
            id = CLOCK_MONOTONIC;
            break;
        case TIME_CLOCK_MONOTONIC_COARSE:
            id = CLOCK_MONOTONIC_COARSE;
            break;
        case TIME_CLOCK_MONOTONIC_RAW:
            id = CLOCK_MONOTONIC_RAW;
            break;
        case TIME_CLOCK_BOOTTIME:
            id = CLOCK_BOOTTIME;
            break;
        case TIME_CLOCK_PROCESS_CPUTIME_ID:
            id = CLOCK_PROCESS_CPUTIME_ID;
            break;
        case TIME_CLOCK_THREAD_CPUTIME_ID:
            id = CLOCK_THREAD_CPUTIME_ID;
            break;
        default:
            id = CLOCK_REALTIME;
            break;
        }
        bool result = ::clock_gettime(id, &ts) == 0 ? true : false;
        if (result) {
            mMsecs = ts.tv_sec % SECS_PER_DAY * 1000 + ts.tv_nsec / 1000 / 1000;
        }
        return result;
    }
    return false;
}

bool Time::setTime(int h, int m, int s, int ms)
{
    if (!isValid(h, m, s, ms)) {
        mMsecs = NullTime;
        return false;
    }
    mMsecs = (h * SECS_PER_HOUR + m * SECS_PER_MIN + s) * 1000 + ms;
    return true;
}

// 12:34:56
bool Time::fromString(const std::string &time)
{
    if (time.length() != 8) {
        return false;
    }

    int c0 = time.at(0) - '0';
    int c1 = time.at(1) - '0';
    char c2 = time.at(2);
    int c3 = time.at(3) - '0';
    int c4 = time.at(4) - '0';
    char c5 = time.at(5);
    int c6 = time.at(6) - '0';
    int c7 = time.at(7) - '0';

    if (c2 != ':' || c5 != ':') {
        return false;
    }

    if (c0 < 0 || c0 > 2
        || c1 < 0 || c1 > 9
        || c3 < 0 || c3 > 5
        || c4 < 0 || c4 > 9
        || c6 < 0 || c6 > 5
        || c7 < 0 || c7 > 9) {
        return false;
    }

    int hour = c0 * 10 + c1;
    int min = c3 * 10 + c4;
    int sec = c6 * 10 + c7;

    if (isValid(hour, min, sec, 0)) {
        mMsecs = hour * MSECS_PER_HOUR + min * MSECS_PER_HOUR + sec * MSECS_PER_MIN;
        return true;
    }

    mMsecs = NullTime;
    return false;
}

std::string Time::toString() const
{
    char buf[8] = { 0 };
    ::snprintf(buf, 8, "%02d-%02d-%02d", hour(), minute(), second());
    return std::string(buf);
}

bool Time::isNull() const
{
    return NullTime == mMsecs;
}

bool Time::isValid() const
{
    return mMsecs > NullTime && mMsecs < MSECS_PER_DAY;
}

int Time::hour() const
{
    if (!isValid()) {
        return -1;
    }
    return msecs() / MSECS_PER_HOUR;
}

int Time::minute() const
{
    if (!isValid()) {
        return -1;
    }
    return (msecs() % MSECS_PER_HOUR) / MSECS_PER_HOUR;
}

int Time::second() const
{
    if (!isValid()) {
        return -1;
    }
    return (msecs() / 1000) % SECS_PER_MIN;
}

int Time::msec() const
{
    if (!isValid()) {
        return -1;
    }
    return msecs() % 1000;
}

void Time::start()
{
    makeCurrent();
}

int Time::elapsed()
{
    int old = mMsecs;
    makeCurrent();
    return mMsecs - old;
}

bool Time::isValid(int h, int m, int s, int ms) const
{
    if (0 <= h && h < 24 && 0 <= m && m < 60 && 0 <= s && s < 60 && 0 <= ms && ms < 1000) {
        return true;
    }
    return false;
}

int Time::msecs() const
{
    return NullTime == mMsecs ? 0 : mMsecs;
}

//==================================================
//==================================================

DateTime::DateTime()
{

}

DateTime::DateTime(const Date &date, const Time &time)
    : mDate(date)
    , mTime(time)
{

}

DateTime::DateTime(const DateTime &dateTime)
    : mDate(dateTime.mDate)
    , mTime(dateTime.mTime)
{

}

DateTime& DateTime::operator=(const DateTime &dateTime)
{
    mDate = dateTime.mDate;
    mTime = dateTime.mTime;
    return *this;
}

DateTime::~DateTime()
{

}

DateTime DateTime::CurrentDateTime()
{
    DateTime dt;
    dt.makeCurrent(TIME_CLOCK_REALTIME);
    return dt;
}

void DateTime::makeCurrent(TimeType type)
{
    mDate.makeCurrent(type);
    mTime.makeCurrent(type);
}

void DateTime::setDate(const Date &date)
{
    mDate = date;
}

void DateTime::setTime(const Time &time)
{
    mTime = time;
}

void DateTime::setDateTime(const Date &date, const Time &time)
{
    setDate(date);
    setTime(time);
}

// 1970-01-01 12:34:56
bool DateTime::fromString(const std::string &dateTime)
{
    if (dateTime.length() != 19) {
        return false;
    }

    if (dateTime.at(11) != ' ') {
        return false;
    }

    std::string date = dateTime.substr(0, 10);
    std::string time = dateTime.substr(11, 8);

    if (mDate.fromString(date) && mTime.fromString(time)) {
        return true;
    }

    return false;
}

std::string DateTime::toString() const
{
    std::string str = mDate.toString();
    str.append(" ");
    str.append(mTime.toString());
    return str;
}

bool DateTime::isNull() const
{
    return mDate.isNull() && mTime.isNull();
}

bool DateTime::isValid() const
{
    return mDate.isValid() && mTime.isValid();
}

Date DateTime::date() const
{
    return mDate;
}

Time DateTime::time() const
{
    return mTime;
}

MU_END_NAMESPACE
