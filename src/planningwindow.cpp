#include "planningwindow.h"
#include "ui_planningwindow.h"
#include "plandialog.h"

#include "views/calendar/basiccalendarmodel.h"
#include "views/calendar/basiccalendaritem.h"

#include "sql/session.h"
#include "sql/plannedsession.h"
#include "sql/managers/plannedsessionsmanager.h"

#include <QDebug>
#include <QTimer>

PlanningWindow::PlanningWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlanningWindow),
    mCalendarModel(new BasicCalendarModel(QDate::currentDate(), QDate::currentDate(), this)),
    mManager(new PlannedSessionsManager(this))
{
    ui->setupUi(this);
    ui->planningView->setModel(mCalendarModel);

    connect(ui->nextAction, SIGNAL(triggered()), ui->planningView, SLOT(next()));
    connect(ui->backAction, SIGNAL(triggered()), ui->planningView, SLOT(previous()));

    connect(ui->planningView, SIGNAL(itemClicked(QDate, int)), this, SLOT(onItemClicked(QDate, int)));

    const QList<PlannedSession> planedSessions = mManager->selectPlannedSessions();
    QList<BasicCalendarItem *> items;
    foreach (const PlannedSession &ps, planedSessions) {
        BasicCalendarItem *item = new BasicCalendarItem(ps.date, ps.session.name);
        item->setStartTime(ps.startTime);
        item->setEndTime(ps.endTime);
        items << item;
        mPlannedSessions[item] = ps;
    }

    mCalendarModel->appendItems(items);
}

PlanningWindow::~PlanningWindow()
{
    delete ui;
}

void PlanningWindow::on_planifyAction_triggered()
{
    PlanDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        PlannedSession ps = dialog.plannedSession();

        if (mManager->createPlannedSession(ps)) {
            BasicCalendarItem *item = new BasicCalendarItem(ps.date, ps.session.name);
            item->setStartTime(ps.startTime);
            item->setEndTime(ps.endTime);
            mCalendarModel->appendItem(item);

            mPlannedSessions[item] = ps;
        }
    }
}

void PlanningWindow::onItemClicked(const QDate &date, int row)
{
    ui->editAction->setEnabled(true);
    ui->removeAction->setEnabled(true);

    mCurrentItem = mCalendarModel->item(date, row);
}

void PlanningWindow::on_removeAction_triggered()
{
    const PlannedSession &ps = mPlannedSessions.value(mCurrentItem);
    if (mManager->removePlannedSessionById(ps.id)) {
        mCalendarModel->removeItem(mCurrentItem);
        mCurrentItem = 0;

        ui->editAction->setEnabled(false);
        ui->removeAction->setEnabled(false);
    }
}

void PlanningWindow::on_editAction_triggered()
{
     const PlannedSession &ps = mPlannedSessions.value(mCurrentItem);
     PlanDialog dialog(ps);
     if (dialog.exec() == QDialog::Accepted) {
        const PlannedSession &session = dialog.plannedSession();
        if (mManager->updatePlannedSession(session)) {
            mCalendarModel->removeItem(mCurrentItem);
            BasicCalendarItem *item = new BasicCalendarItem(session.date, session.session.name);
            item->setStartTime(session.startTime);
            item->setEndTime(session.endTime);

            mCalendarModel->appendItem(item);
            mCurrentItem = 0;

            ui->editAction->setEnabled(false);
            ui->removeAction->setEnabled(false);
        }
     }
}
