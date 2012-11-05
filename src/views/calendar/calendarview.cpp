#include "calendarview.h"

#include "abstractcalendarmodel.h"
#include "basiccalendarmodel.h"

#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>

#include <QDate>
#include <QDebug>
#include <QMetaObject>

static const int NumberOfColumns = 7;
static const int NumberOfRows = 6;
static const int NumberOfItems = NumberOfColumns * NumberOfRows;

CalendarView::CalendarView(QWidget *parent) :
    QDeclarativeView(parent),
    mViewType(MonthView),
    mModel(0),
    mDate(QDate::currentDate())
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
    fetchTodayModel();
    QDeclarativeContext *context = rootContext();
    context->setContextProperty("calendarModel", mModel);
    context->setContextProperty("view", this);
    setSource(QUrl::fromLocalFile("src/views/calendar/qml/CalendarView.qml"));

    QObject *item = rootObject();
    connect(item, SIGNAL(clicked(QDateTime, int)), this, SLOT(onClicked(QDateTime, int)));
}

AbstractCalendarModel *CalendarView::model() const
{
    return mModel;
}

void CalendarView::setDate(const QDate &date)
{
    mDate = date;
}

QModelIndex CalendarView::currentIndex() const
{
    return mCurrentIndex;
}

void CalendarView::previous()
{
    const int previousMonthNumber = mDate.month() == 1 ? 12 : mDate.month() - 1;
    const int previousYear = previousMonthNumber == 12 ? mDate.year() - 1 : mDate.year();
    QDate firstPreviousMonthDay(previousYear, previousMonthNumber, 1);
    const int numberOfDays = firstPreviousMonthDay.daysInMonth();

    const int leftDayPadding = firstPreviousMonthDay.dayOfWeek() - 1;
    const int rightPadding = NumberOfItems - leftDayPadding - numberOfDays;

    const QDate startDate = firstPreviousMonthDay.addDays(-leftDayPadding);
    const QDate endDate = firstPreviousMonthDay.addDays(numberOfDays + rightPadding);

    mModel->fetch(startDate, endDate);
    QDeclarativeContext *context = rootContext();
    context->setContextProperty("currentDate", firstPreviousMonthDay);
    context->setContextProperty("currentMonthNumber", previousMonthNumber);
    mDate = firstPreviousMonthDay;
}

void CalendarView::next()
{
    const int nextMonthNumber = mDate.month() == 12 ? 1 : mDate.month() + 1;
    const int nextYear = nextMonthNumber == 1 ? mDate.year() + 1 : mDate.year();
    QDate firstNextMonthDay(nextYear, nextMonthNumber, 1);
    const int numberOfDays = firstNextMonthDay.daysInMonth();

    const int leftDayPadding = firstNextMonthDay.dayOfWeek() - 1;
    const int rightPadding = NumberOfItems - leftDayPadding - numberOfDays;

    const QDate startDate = firstNextMonthDay.addDays(-leftDayPadding);
    const QDate endDate = firstNextMonthDay.addDays(numberOfDays + rightPadding);

    mModel->fetch(startDate, endDate);
    QDeclarativeContext *context = rootContext();
    context->setContextProperty("currentDate", firstNextMonthDay);
    context->setContextProperty("currentMonthNumber", nextMonthNumber);
    mDate = firstNextMonthDay;
}

void CalendarView::goToday()
{
    mDate = QDate::currentDate();
    fetchTodayModel();
}

void CalendarView::onClicked(const QDateTime &dateTime, int row)
{
    const QModelIndex index = mModel->index(dateTime.date());
    mCurrentIndex = index;
    emit itemClicked(dateTime.date(), row);
}

void CalendarView::fetchTodayModel()
{
    const QDate today = QDate::currentDate();
    QDate firstPreviousMonthDay(today.year(), today.month(), 1);
    const int numberOfDays = firstPreviousMonthDay.daysInMonth();

    const int leftDayPadding = firstPreviousMonthDay.dayOfWeek() - 1;
    const int rightPadding = NumberOfItems - leftDayPadding - numberOfDays;

    const QDate startDate = firstPreviousMonthDay.addDays(-leftDayPadding);
    const QDate endDate = firstPreviousMonthDay.addDays(numberOfDays + rightPadding);

    mModel->fetch(startDate, endDate);
    QDeclarativeContext *context = rootContext();
    context->setContextProperty("currentDate", mDate);
    context->setContextProperty("currentMonthNumber", mDate.month());
}

