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
    mCalendarModel(new BasicCalendarModel(QDate::currentDate(), this))
{
    ui->setupUi(this);
    ui->planningView->setModel(mCalendarModel);

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
