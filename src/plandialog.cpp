#include "plandialog.h"
#include "ui_plandialog.h"

#include "sql/models/sessionsmodel.h"

PlanDialog::PlanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDialog),
    mSessionsModel(new SessionsModel(this))
{
    ui->setupUi(this);
    ui->cmbTrainings->setModel(mSessionsModel);
    ui->dateEdit->setDate(QDate::currentDate());
}

PlanDialog::~PlanDialog()
{
    delete ui;
}

PlannedSession PlanDialog::plannedSession() const
{
    PlannedSession session;
    const int currentIndex = ui->cmbTrainings->currentIndex();
    session.session = mSessionsModel->session(currentIndex);
    session.date = ui->dateEdit->date();
    session.startTime = ui->startTimeEdit->time();
    session.endTime = ui->endTimeEdit->time();

    return session;
}

void PlanDialog::on_btnToday_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
}
