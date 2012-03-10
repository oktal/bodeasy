#ifndef CALENDARMODEL_H
#define CALENDARMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QVariant>

enum ItemRole { SubjectRole = Qt::UserRole + 1, DescriptionRole, StartTimeRole, EndTimeRole,
                BackgroundRole, ForegroundRole };

class AbstractCalendarModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Role { DayNumberRole = Qt::UserRole + 1, MonthNumberRole, MonthNameRole, DateRole, ItemsRole };

    explicit AbstractCalendarModel(const QDate &date, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    virtual int itemsCount(const QDate &date) const = 0;
    virtual QVariant calendarItemData(const QDate &date, int row, int role) const = 0;

private:
    int rowForFirstOfMonth() const;
    int monthDaysCount(int monthNumber) const;
    QDate dateForIndex(const QModelIndex &index) const;
    QDate mDate;
    
};

#endif // CALENDARMODEL_H
