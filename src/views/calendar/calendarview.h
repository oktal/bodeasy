#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QDeclarativeView>
#include <QModelIndex>

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

signals:
    void clicked(const QModelIndex &index);

public slots:
    void previous();
    void next();

private slots:
     void itemClicked(const QDateTime &dateTime);

private:
    void fetchTodayModel();
    ViewType mViewType;
    AbstractCalendarModel *mModel;
    QDate mDate;
    
};

#endif // CALENDARVIEW_H
