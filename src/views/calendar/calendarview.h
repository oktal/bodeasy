#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QDeclarativeView>
#include <QModelIndex>

class AbstractCalendarModel;

class CalendarView : public QDeclarativeView
{
    Q_OBJECT
public:

    enum ViewType { Day, Week, Month };

    explicit CalendarView(QWidget *parent = 0);

    void setViewType(ViewType type);
    ViewType viewType() const;

    void setModel(AbstractCalendarModel *model);
    AbstractCalendarModel *model() const;

    
private:
    ViewType mViewType;
    AbstractCalendarModel *mModel;
    
};

#endif // CALENDARVIEW_H
