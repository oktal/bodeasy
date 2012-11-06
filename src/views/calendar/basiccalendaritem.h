#ifndef BASICCALENDARITEM_H
#define BASICCALENDARITEM_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QColor>

class BasicCalendarItem
{
public:
    BasicCalendarItem();
    BasicCalendarItem(const QDate &date, const QString &subject);

    void setDate(const QDate &date);
    QDate date() const;

    void setSubject(const QString &subject);
    QString subject() const;

    void setStartTime(const QTime &time);
    QTime startTime() const;

    void setEndTime(const QTime &time);
    QTime endTime() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void setForegroundColor(const QColor &color);
    QColor foreGroundColor() const;

private:
    QDate mDate;
    QString mSubject;
    QTime mStartTime;
    QTime mEndTime;
    QColor mBackgroundColor;
    QColor mForeGroundColor;

};

#endif // BASICCALENDARITEM_H
