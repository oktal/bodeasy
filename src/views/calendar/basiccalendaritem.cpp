#include "basiccalendaritem.h"

BasicCalendarItem::BasicCalendarItem()
{
}

BasicCalendarItem::BasicCalendarItem(const QDate &date, const QString &subject) :
    mDate(date),
    mSubject(subject),
    mBackgroundColor(Qt::red),
    mForeGroundColor(64, 0, 0)
{
}

void BasicCalendarItem::setDate(const QDate &date)
{
    mDate = date;
}

QDate BasicCalendarItem::date() const
{
    return mDate;
}

void BasicCalendarItem::setSubject(const QString &subject)
{
    mSubject = subject;
}

QString BasicCalendarItem::subject() const
{
    return mSubject;
}

void BasicCalendarItem::setStartTime(const QTime &time)
{
    Q_ASSERT(time <= mEndTime);
    mStartTime = time;
}

QTime BasicCalendarItem::startTime() const
{
    return mStartTime;
}

void BasicCalendarItem::setEndTime(const QTime &time)
{
    Q_ASSERT(mEndTime >= mStartTime);
    mEndTime = time;
}

QTime BasicCalendarItem::endTime() const
{
    return mEndTime;
}

void BasicCalendarItem::setBackgroundColor(const QColor &color)
{
    mBackgroundColor = color;
}

QColor BasicCalendarItem::backgroundColor() const
{
    return mBackgroundColor;
}

void BasicCalendarItem::setForegroundColor(const QColor &color)
{
    mForeGroundColor = color;
}

QColor BasicCalendarItem::foreGroundColor() const
{
    return mForeGroundColor;
}
