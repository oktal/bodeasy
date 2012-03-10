#include "abstractcalendarmodel.h"
#include "calendaritem.h"

#include <QDate>
#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDebug>

static const int NumberOfColumns = 7;
static const int NumberOfRows = 6;
static const int NumberOfItems = NumberOfColumns * NumberOfRows;

Q_DECLARE_METATYPE(QList<QObject *>)

AbstractCalendarModel::AbstractCalendarModel(const QDate &date, QObject *parent) :
    QAbstractListModel(parent),
    mDate(date)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AbstractCalendarModel::DayNumberRole] = "dayNumber";
    roleNames[AbstractCalendarModel::MonthNumberRole] = "monthNumber";
    roleNames[AbstractCalendarModel::MonthNameRole] = "monthName";
    roleNames[AbstractCalendarModel::DateRole] = "date";
    roleNames[AbstractCalendarModel::ItemsRole] = "items";
    setRoleNames(roleNames);
}

int AbstractCalendarModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : NumberOfItems;
}

QVariant AbstractCalendarModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= NumberOfItems) {
        return QVariant();
    }

    const QDate indexDate = dateForIndex(index);

    switch (role) {
    case AbstractCalendarModel::DateRole:
        return indexDate;
    case AbstractCalendarModel::DayNumberRole:
        return indexDate.day();
        break;
    case AbstractCalendarModel::MonthNameRole:
        return QDate::shortMonthName(indexDate.month());
        break;
    case AbstractCalendarModel::MonthNumberRole:
        return indexDate.month();
        break;
    case AbstractCalendarModel::ItemsRole:
    {
        QList<QObject *> items;
        const int count = itemsCount(indexDate);
        for (int i = 0; i < count; ++i) {
            CalendarItem *item = new CalendarItem;
            item->setSubject(calendarItemData(indexDate, i, SubjectRole).toString());
            item->setStartTime(calendarItemData(indexDate, i, StartTimeRole).toTime());
            item->setEndTime(calendarItemData(indexDate, i, EndTimeRole).toTime());
            item->setBackgroundColor(calendarItemData(indexDate, i, BackgroundRole).value<QColor>());
            item->setForeGroundColor(calendarItemData(indexDate, i, ForegroundRole).value<QColor>());
            items << item;
        }
        return QVariant::fromValue(items);
    }
    }

    return QVariant();
}


int AbstractCalendarModel::rowForFirstOfMonth() const
{
    const QDate first(mDate.year(), mDate.month(), 1);
    const int dayOfWeek = first.dayOfWeek();

    return dayOfWeek;
}

int AbstractCalendarModel::monthDaysCount(int monthNumber) const
{
    Q_ASSERT(monthNumber >= 1 && monthNumber <= 12);

    QDate date(mDate.year(), monthNumber, 1);
    return date.daysInMonth();
}

QDate AbstractCalendarModel::dateForIndex(const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    const int row = index.row();
    const int first = rowForFirstOfMonth();
    const int firstRow = first - 1;
    const int daysInCurrentMonth = monthDaysCount(mDate.month());

    QDate date;
    if (row < firstRow) {
        const int previousDaysNumberMonth = monthDaysCount(mDate.month() - 1);
        const int dayOfMonth = previousDaysNumberMonth - (firstRow - row) + 1;
        date.setDate(mDate.year(), mDate.month() - 1, dayOfMonth);
    }
    else if (row >= daysInCurrentMonth + firstRow) {
        const int dayOfMonth = row - daysInCurrentMonth - firstRow + 1;
        date.setDate(mDate.year(), mDate.month() + 1, dayOfMonth);
    }
    else
    {
        date.setDate(mDate.year(), mDate.month(), (row - firstRow) + 1);
    }

    return date;
}
