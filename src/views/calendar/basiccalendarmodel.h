#ifndef BASICCALENDARMODEL_H
#define BASICCALENDARMODEL_H

#include "abstractcalendarmodel.h"
#include <QList>
#include <QMap>

class BasicCalendarItem;

class BasicCalendarModel : public AbstractCalendarModel
{
public:
    BasicCalendarModel(const QDate &startDate, const QDate &endDate, QObject *parent = 0);
    ~BasicCalendarModel();

    int itemsCount(const QDate &date) const;
    QVariant calendarItemData(const QDate &date, int row, int role) const;

    void appendItems(const QList<BasicCalendarItem *> items);
    void appendItem(BasicCalendarItem *item);

    void setItem(const QDate &date, int row, BasicCalendarItem *item);
    QModelIndex indexFromItem(BasicCalendarItem *item) const;

    BasicCalendarItem *item(const QDate &date, int row) const;
    QList<BasicCalendarItem *> items(const QDate &date) const;

private:
    QList<BasicCalendarItem *> mItems;
    QMap<QDate, QList<BasicCalendarItem *> > mMappedItems;


};

#endif // BASICCALENDARMODEL_H
