#include "usersdialog.h"
#include "ui_usersdialog.h"

#include "settings.h"

#include <QSettings>

#include "sql/models/usersmodel.h"
#include "sql/user.h"

#include <QMessageBox>

UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersDialog),
    m_model(new UsersModel(this))
{
    ui->setupUi(this);
    ui->cmbUsers->setModel(m_model);
    ui->btnDelete->setEnabled(m_model->rowCount() > 0);

    QSettings settings(qApp->organizationName(), qApp->applicationName());
    const bool selectLastUser = settings.value(SETTING_LASTUSER_STARTUP, false).toBool();
    if (selectLastUser)
    {
        const QString lastUser = settings.value(SETTING_LASTUSERNAME).toString();
        ui->cmbUsers->setCurrentIndex(
                    ui->cmbUsers->findText(lastUser));
    }
}

UsersDialog::~UsersDialog()
{
    delete ui;
}

void UsersDialog::on_btnAdd_clicked()
{
    const QString newUser = ui->txtUser->text().trimmed();
    const bool hasUser = !m_model->match(m_model->index(0,0),Qt::DisplayRole,newUser,1,Qt::MatchFixedString | Qt::MatchRecursive).isEmpty();

    if (hasUser)
    {
        QMessageBox::information(this,
                                 trUtf8("Ajout d'utilisateur"),
                                 trUtf8("L'utilisateur %1 existe déjà , impossible de l'ajouter à nouveau.")
                                 .arg(newUser));
        return;
    }

    User user;
    user.id = -1;
    user.name = newUser;
    if (m_model->addUser(user))
    {
        ui->btnDelete->setEnabled(m_model->rowCount() > 0);
        ui->txtUser->clear();
        ui->btnAdd->setEnabled(false);
        ui->cmbUsers->setCurrentIndex(m_model->userIndex(user).row());
    }
}

void UsersDialog::on_btnDelete_clicked()
{
    int r = QMessageBox::question(this,
                                  tr("Suppression d'utilisateur"),
                                  trUtf8("Supprimer définitivement l'utilisateur %1 ?")
                                  .arg(ui->cmbUsers->currentText()),
                                  QMessageBox::Yes, QMessageBox::Cancel);

    if (r == QMessageBox::Yes)
    {
        const QModelIndex index = m_model->index(ui->cmbUsers->currentIndex());
        m_model->removeUser(m_model->user(index));
        ui->btnDelete->setEnabled(m_model->rowCount() > 0);
    }
}

void UsersDialog::on_txtUser_textEdited()
{
    ui->btnAdd->setEnabled(!ui->txtUser->text().isEmpty());
}


void UsersDialog::accept()
{
    if (ui->cmbUsers->currentIndex() != -1)
        QDialog::accept();
}

qint64 UsersDialog::userId() const
{
    return ui->cmbUsers->itemData(ui->cmbUsers->currentIndex()).toLongLong();
}
