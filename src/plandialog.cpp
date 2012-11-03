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

Session PlanDialog::session() const
{
    const int currentIndex = ui->cmbTrainings->currentIndex();
    return mSessionsModel->session(currentIndex);
}

QDate PlanDialog::date() const
{
    return ui->dateEdit->date();
}

void PlanDialog::on_btnToday_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
}
