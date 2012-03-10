#include "calendaritem.h"

#include <QDebug>


CalendarItem::CalendarItem(const QString &subject, QObject *parent) : QObject(parent),
    mSubject(subject)
{
}

CalendarItem::CalendarItem(QObject *parent) : QObject(parent) {

}

CalendarItem::~CalendarItem()
{
    qDebug() << "BLAH";
}

QString CalendarItem::subject() const {
    return mSubject;
}

void CalendarItem::setSubject(const QString &subject) {
    if (mSubject != subject) {
        mSubject = subject;
        emit subjectChanged();
    }
}

QTime CalendarItem::endTime() const
{
    return mEndTime;
}

void CalendarItem::setEndTime(const QTime &time)
{
    Q_ASSERT(time >= mStartTime);
    if (mEndTime != time) {
        mEndTime = time;
        emit endTimeChanged();
    }
}

QTime CalendarItem::startTime() const
{
    return mStartTime;
}

void CalendarItem::setStartTime(const QTime &time)
{
    Q_ASSERT(time <= mEndTime);
    if (mStartTime != time) {
        mStartTime = time;
        emit startTimeChanged();
    }
}

QColor CalendarItem::backgroundColor() const
{
    return mBackgroundColor;
}

void CalendarItem::setBackgroundColor(const QColor &color)
{
    if (color != mBackgroundColor) {
        mBackgroundColor = color;
        emit backgroundColorChanged();
    }
}

QColor CalendarItem::foreGroundColor() const
{
    return mForeGroundColor;
}

void CalendarItem::setForeGroundColor(const QColor &color)
{
    if (mForeGroundColor != color) {
        mForeGroundColor = color;
        emit foreGroundColorChanged();
    }
}
