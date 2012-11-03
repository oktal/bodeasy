#include "planningwindow.h"
#include "ui_planningwindow.h"
#include "plandialog.h"

#include "views/calendar/basiccalendarmodel.h"
#include "views/calendar/basiccalendaritem.h"

#include "sql/session.h"

#include <QDebug>
#include <QTimer>

PlanningWindow::PlanningWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlanningWindow),
    mCalendarModel(new BasicCalendarModel(QDate::currentDate(), QDate::currentDate(), this))
{
    ui->setupUi(this);
    ui->planningView->setModel(mCalendarModel);

    connect(ui->nextAction, SIGNAL(triggered()), ui->planningView, SLOT(next()));
    connect(ui->backAction, SIGNAL(triggered()), ui->planningView, SLOT(previous()));

    connect(ui->planningView, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));

}

PlanningWindow::~PlanningWindow()
{
    delete ui;
}

void PlanningWindow::on_planifyAction_triggered()
{
    PlanDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        const Session &s = dialog.session();
        const QDate &date = dialog.date();

        BasicCalendarItem *item = new BasicCalendarItem(date, s.name);
        mCalendarModel->appendItem(item);
    }
}

void PlanningWindow::onItemClicked(const QModelIndex &index)
{
    ui->editAction->setEnabled(true);

    const QDate date = mCalendarModel->dateForIndex(index);
    qDebug() << date;
}
