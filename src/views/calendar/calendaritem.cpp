#include "calendaritem.h"

#include <QDebug>


CalendarItem::CalendarItem(const QString &subject, QObject *parent) : QObject(parent),
    mSubject(subject)
{
}

CalendarItem::CalendarItem(int row, QObject *parent) :
    QObject(parent),
    mRow(row)
{
}

CalendarItem::~CalendarItem()
{

}

int CalendarItem::row() const
{
    return mRow;
}

QDate CalendarItem::date() const
{
    return mDate;
}

void CalendarItem::setDate(const QDate &date)
{
    if (mDate != date) {
        mDate = date;
        emit dateChanged();
    }
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
