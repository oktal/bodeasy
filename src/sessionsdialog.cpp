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
    ui->lstSessions->setProportionalSectionSizes(Qt::Horizontal, true);
    ui->lstSessions->header()->setResizeMode(QHeaderView::ResizeToContents);
    ui->lstSessions->header()->setResizeMode(0, QHeaderView::Stretch);

    ui->lstExercises->setModel(sessionExercisesModel);
    ui->lstExercises->setItemDelegate(new SessionExercisesDelegate(this));
    ui->lstExercises->header()->setResizeMode(QHeaderView::ResizeToContents);
    ui->lstExercises->header()->setResizeMode(0, QHeaderView::Stretch);
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
        ui->lstSessions->setCurrentIndex(sessionsModel->index(s));
    }
}

void SessionsDialog::on_btnModify_clicked()
{
    Session s(ui->txtSessionName->text(),
              ui->txtObjective->toPlainText(),
              sessionExercisesModel->sessionExercises());
    s.id = sessionsModel->session(ui->lstSessions->currentIndex()).id;

    if (sessionsModel->updateSession(s))
    {
        emit sessionUpdated(s.id);
    }
}

void SessionsDialog::on_btnDelete_clicked()
{
    const qint64 id = sessionsModel->session(ui->lstSessions->currentIndex()).id;
    if (sessionsModel->removeRow(ui->lstSessions->currentIndex().row()))
    {
        ui->btnReset->click();
        emit sessionDeleted(id);
    }
}

void SessionsDialog::on_btnReset_clicked()
{
    ui->txtSessionName->clear();
    ui->chkObjective->setChecked(false);
    ui->txtObjective->clear();
    ui->chkLoop->setChecked(false);
    ui->txtLoop->clear();
    sessionExercisesModel->setSessionExercises(QList<Session::SessionExercise>());
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
    const Session s = sessionsModel->session(ui->lstSessions->currentIndex());
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
