#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <QObject>
#include <QColor>
#include <QString>
#include <QTime>

class CalendarItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(int row READ row)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor foreGroundColor READ foreGroundColor WRITE setForeGroundColor NOTIFY foreGroundColorChanged)
public:
    CalendarItem(int row, QObject *parent = 0);
    CalendarItem(const QString &subject, QObject *parent = 0);
    ~CalendarItem();

    int row() const;

    QDate date() const;
    void setDate(const QDate &date);

    QString subject() const;
    void setSubject(const QString &subject);

    QTime endTime() const;
    void setEndTime(const QTime &time);

    QTime startTime() const;
    void setStartTime(const QTime &time);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor foreGroundColor() const;
    void setForeGroundColor(const QColor &color);

signals:
    void dateChanged();
    void subjectChanged();
    void startTimeChanged();
    void endTimeChanged();
    void backgroundColorChanged();
    void foreGroundColorChanged();

private:
    int mRow;
    QDate mDate;
    QString mSubject;
    QTime mStartTime;
    QTime mEndTime;
    QColor mBackgroundColor;
    QColor mForeGroundColor;
};


#endif // CALENDARITEM_H
