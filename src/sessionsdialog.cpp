#include "sessionsdialog.h"
#include "ui_sessionsdialog.h"
#include "sql/models/sessionsmodel.h"
#include "sql/models/sessionexercisesmodel.h"
#include "sql/models/sessionexercisesdelegate.h"

#include <QDebug>
#include <QMessageBox>

SessionsDialog::SessionsDialog(SessionsModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionsDialog),
    sessionsModel(model), sessionExercisesModel(new SessionExercisesModel)
{
    ui->setupUi(this);
    ui->lstSessions->setModel(sessionsModel);
    int const Width = ui->lstSessions->horizontalHeader()->width();
    ui->lstSessions->horizontalHeader()->resizeSection(0, (Width * 80) / 100);
    ui->lstSessions->horizontalHeader()->resizeSection(1, (Width * 20) / 100);
    ui->lstSessions->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->lstExercises->setModel(sessionExercisesModel);
    ui->lstExercises->setItemDelegate(new SessionExercisesDelegate(this));
    ui->lstExercises->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

SessionsDialog::~SessionsDialog()
{
    delete ui;
}


void SessionsDialog::on_btnAdd_clicked()
{
    Session s(ui->txtSessionName->text(),
              ui->txtObjective->toPlainText(),
              sessionExercisesModel->sessionExercises());
    if (sessionsModel->index(s).isValid())
    {
        QMessageBox::critical(this, trUtf8("Erreur"),
                              trUtf8("La session que vous voulez ajouter existe déjà"));
    }

    if (sessionsModel->addSession(s))
    {
        ui->lstSessions->selectRow(sessionsModel->rowCount());
    }
}

void SessionsDialog::on_btnModify_clicked()
{
    Session s(ui->txtSessionName->text(),
              ui->txtObjective->toPlainText(),
              sessionExercisesModel->sessionExercises());
    s.id = sessionsModel->session(sessionsModel->index(ui->lstSessions->currentIndex().row(), 0)).id;

    if (sessionsModel->updateSession(s))
    {

    }
}

void SessionsDialog::on_btnDelete_clicked()
{
    if (sessionsModel->removeRow(ui->lstSessions->currentIndex().row()))
    {
        ui->txtSessionName->clear();
        ui->chkObjective->setChecked(false);
        ui->txtObjective->setEnabled(false);
        ui->txtObjective->clear();
        sessionExercisesModel->setSessionExercises(QList<Session::SessionExercise>());
    }
}

void SessionsDialog::on_btnAddExercise_clicked()
{
    sessionExercisesModel->insertRow(sessionExercisesModel->rowCount());
}

void SessionsDialog::on_btnDeleteExercise_clicked()
{
    sessionExercisesModel->removeRow(ui->lstExercises->currentIndex().row());
}

void SessionsDialog::on_lstSessions_clicked()
{
    const Session &s = sessionsModel->session(sessionsModel->index(ui->lstSessions->currentIndex().row(), 0));
    ui->txtSessionName->setText(s.name);
    ui->chkObjective->setChecked(!s.objective.isNull());
    ui->txtObjective->setEnabled(!s.objective.isNull());
    ui->txtObjective->setPlainText(s.objective);
    sessionExercisesModel->setSessionExercises(s.exercises);

    ui->btnModify->setEnabled(true);
    ui->btnDelete->setEnabled(true);
}

void SessionsDialog::on_txtSessionName_textEdited(const QString &text)
{
    ui->btnAdd->setEnabled(!text.isEmpty());
}
