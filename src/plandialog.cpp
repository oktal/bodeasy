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

    mPlannedSession.session = mSessionsModel->session(0);
    mPlannedSession.date = QDate::currentDate();
    mPlannedSession.startTime = QTime(0, 0, 0);
    mPlannedSession.endTime = QTime(0, 0, 0);
}

PlanDialog::PlanDialog(const PlannedSession &ps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDialog),
    mSessionsModel(new SessionsModel(this)),
    mPlannedSession(ps)
{
    ui->setupUi(this);
    ui->cmbTrainings->setModel(mSessionsModel);

    ui->dateEdit->setDate(ps.date);

    const QModelIndex sessionIndex = mSessionsModel->index(ps.session);
    ui->cmbTrainings->setCurrentIndex(sessionIndex.row());

    ui->startTimeEdit->setTime(ps.startTime);
    ui->endTimeEdit->setTime(ps.endTime);
}

PlanDialog::~PlanDialog()
{
    delete ui;
}

PlannedSession PlanDialog::plannedSession() const
{
    return mPlannedSession;
}

void PlanDialog::on_btnToday_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
}

void PlanDialog::on_cmbTrainings_activated(int index)
{
    const Session &s = mSessionsModel->session(index);
    mPlannedSession.session = s;
}

void PlanDialog::on_dateEdit_dateChanged(const QDate &date)
{
    mPlannedSession.date = date;
}

void PlanDialog::on_startTimeEdit_timeChanged(const QTime &time)
{
    mPlannedSession.startTime = time;
}

void PlanDialog::on_endTimeEdit_timeChanged(const QTime &time)
{
    mPlannedSession.endTime = time;
}
