#include "basiccalendarmodel.h"
#include "basiccalendaritem.h"

#include <QDebug>
#include <QModelIndex>

BasicCalendarModel::BasicCalendarModel(const QDate &date, QObject *parent) :
    AbstractCalendarModel(date, parent),
    mDate(date)
{
}

BasicCalendarModel::~BasicCalendarModel()
{
    qDeleteAll(mItems);
}

int BasicCalendarModel::itemsCount(const QDate &date) const
{
    return items(date).count();
}

QVariant BasicCalendarModel::calendarItemData(const QDate &date, int row, int role) const
{
    const QList<BasicCalendarItem *> &itemsList = items(date);
    if (row >= itemsList.count()) {
        return QVariant();
    }

    const BasicCalendarItem *item = itemsList.at(row);
    switch (role) {
        case SubjectRole:
            return item->subject();
        case StartTimeRole:
            return item->startTime();
        case EndTimeRole:
            return item->endTime();
        case BackgroundRole:
            return item->backgroundColor();
        case ForegroundRole:
            return item->foreGroundColor();
    }

    return QVariant();
}

void BasicCalendarModel::appendItems(const QList<BasicCalendarItem *> items)
{
    emit layoutAboutToBeChanged();
    mItems.append(items);
    foreach (BasicCalendarItem *item, items) {
        const QDate &date = item->date();

        QList<BasicCalendarItem *> &mappedItemsList = mMappedItems[date];
        mappedItemsList.append(item);
    }
    emit layoutChanged();
}

void BasicCalendarModel::appendItem(BasicCalendarItem *item)
{
    appendItems(QList<BasicCalendarItem *>() << item);
}

QModelIndex BasicCalendarModel::indexFromItem(BasicCalendarItem *item) const
{
    Q_ASSERT(item);
    const QDate date = item->date();

    const QDate first(mDate.year(), mDate.month(), 1);
    const int dayOfWeek = first.dayOfWeek();

    if (date.month() == mDate.month()) {
        return index(dayOfWeek + date.day());
    }
    else if (date.month() == mDate.month() - 1) {
        return index(date.day());
    }
    else if (date.month() == mDate.month() + 1) {
        return index(dayOfWeek + first.daysInMonth() + date.day());
    }

    return QModelIndex();
}

BasicCalendarItem *BasicCalendarModel::item(const QDate &date, int row) const
{
    const QList<BasicCalendarItem *> itemsList = items(date);
    return row < itemsList.count() ? itemsList.at(row) : 0;
}

QList<BasicCalendarItem *> BasicCalendarModel::items(const QDate &date) const
{
    return mMappedItems.value(date, QList<BasicCalendarItem *>());
}
