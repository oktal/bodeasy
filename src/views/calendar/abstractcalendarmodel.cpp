#include "abstractcalendarmodel.h"
#include "calendaritem.h"

#include <QDate>
#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDebug>

Q_DECLARE_METATYPE(QList<QObject *>)

AbstractCalendarModel::AbstractCalendarModel(const QDate &startDate, const QDate &endDate,
                                             QObject *parent) :
    QAbstractListModel(parent),
    mStartDate(startDate),
    mEndDate(endDate)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AbstractCalendarModel::DayNumberRole] = "dayNumber";
    roleNames[AbstractCalendarModel::MonthNumberRole] = "monthNumber";
    roleNames[AbstractCalendarModel::YearNumberRole] = "yearNumber";
    roleNames[AbstractCalendarModel::MonthNameRole] = "monthName";
    roleNames[AbstractCalendarModel::DateRole] = "date";
    roleNames[AbstractCalendarModel::ItemsRole] = "items";
    setRoleNames(roleNames);
}

AbstractCalendarModel::~AbstractCalendarModel()
{
    qDeleteAll(mItems);
}

int AbstractCalendarModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mStartDate.daysTo(mEndDate);
}

QVariant AbstractCalendarModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }

    const QDate indexDate = dateForIndex(index);

    switch (role) {
    case AbstractCalendarModel::DateRole:
        return indexDate;
    case AbstractCalendarModel::DayNumberRole:
        return indexDate.day();
        break;
    case AbstractCalendarModel::YearNumberRole:
        return indexDate.year();
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
            CalendarItem *item = new CalendarItem(i);
            item->setDate(indexDate);
            item->setSubject(calendarItemData(indexDate, i, SubjectRole).toString());
            item->setStartTime(calendarItemData(indexDate, i, StartTimeRole).toTime());
            item->setEndTime(calendarItemData(indexDate, i, EndTimeRole).toTime());
            item->setBackgroundColor(calendarItemData(indexDate, i, BackgroundRole).value<QColor>());
            item->setForeGroundColor(calendarItemData(indexDate, i, ForegroundRole).value<QColor>());
            items << item;
        }
        mItems << items;
        return QVariant::fromValue(items);
    }
    }

    return QVariant();
}

void AbstractCalendarModel::fetch(const QDate &startDate, const QDate &endDate)
{
    Q_ASSERT(startDate <= endDate);
    beginResetModel();
    mStartDate = startDate;
    mEndDate = endDate;
    endResetModel();
}

QModelIndex AbstractCalendarModel::index(const QDate &date) const
{
    const int row = mStartDate.daysTo(date);
    return QAbstractListModel::index(row, 0, QModelIndex());
}

QDate AbstractCalendarModel::startDate() const
{
    return mStartDate;
}

QDate AbstractCalendarModel::endDate() const
{
    return mEndDate;
}


QDate AbstractCalendarModel::dateForIndex(const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    const int row = index.row();
    return mStartDate.addDays(row);
}
