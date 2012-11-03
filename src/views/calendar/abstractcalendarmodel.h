#ifndef CALENDARMODEL_H
#define CALENDARMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QVariant>

enum ItemRole { SubjectRole = Qt::UserRole + 1, DescriptionRole, StartTimeRole, EndTimeRole,
                BackgroundRole, ForegroundRole };

class AbstractCalendarModel : public QAbstractListModel
{
    friend class CalendarView;
    Q_OBJECT
public:

    enum Role { DayNumberRole = Qt::UserRole + 1, MonthNumberRole, YearNumberRole,
                MonthNameRole, DateRole, ItemsRole };

    explicit AbstractCalendarModel(const QDate &startDate, const QDate &endDate, QObject *parent = 0);
    ~AbstractCalendarModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    virtual int itemsCount(const QDate &date) const = 0;
    virtual QVariant calendarItemData(const QDate &date, int row, int role) const = 0;

    void fetch(const QDate &startDate, const QDate &endDate);

    using QAbstractListModel::index;
    QModelIndex index(const QDate &date) const;

    QDate dateForIndex(const QModelIndex &index) const;

    QDate startDate() const;
    QDate endDate() const;

private:
    QDate mStartDate;
    QDate mEndDate;
    mutable QList<QObject *> mItems;
    
};

#endif // CALENDARMODEL_H
