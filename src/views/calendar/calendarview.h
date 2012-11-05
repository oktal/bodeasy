#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QDeclarativeView>
#include <QModelIndex>
#include <QPersistentModelIndex>

class AbstractCalendarModel;

class CalendarView : public QDeclarativeView
{
    Q_OBJECT
public:

    enum ViewType { DayView, WeekView, MonthView };

    explicit CalendarView(QWidget *parent = 0);

    void setViewType(ViewType type);
    ViewType viewType() const;

    void setModel(AbstractCalendarModel *model);
    AbstractCalendarModel *model() const;

    void setDate(const QDate &date);

    QModelIndex currentIndex() const;

signals:
    void itemClicked(const QDate &date, int row);

public slots:
    void previous();
    void next();

    void goToday();

private slots:
     void onClicked(const QDateTime &dateTime, int row);

private:
    void fetchTodayModel();
    ViewType mViewType;
    AbstractCalendarModel *mModel;
    QDate mDate;
    QPersistentModelIndex mCurrentIndex;
};

#endif // CALENDARVIEW_H
