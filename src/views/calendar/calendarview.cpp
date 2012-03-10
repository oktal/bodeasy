#include "calendarview.h"

#include "abstractcalendarmodel.h"
#include "basiccalendarmodel.h"

#include <QDeclarativeContext>

CalendarView::CalendarView(QWidget *parent) :
    QDeclarativeView(parent)
{
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setFrameStyle(QFrame::Box);
}

void CalendarView::setViewType(CalendarView::ViewType type)
{
    mViewType = type;
}

CalendarView::ViewType CalendarView::viewType() const
{
    return mViewType;
}

void CalendarView::setModel(AbstractCalendarModel *model)
{
    mModel = model;
    QDeclarativeContext *context = rootContext();
    context->setContextProperty("currentMonthNumber", 3);
    context->setContextProperty("currentDate", QDate::currentDate());
    context->setContextProperty("calendarModel", mModel);
    setSource(QUrl("src/views/calendar/qml/CalendarView.qml"));
}

AbstractCalendarModel *CalendarView::model() const
{
    return mModel;
}

