#include "usersdialog.h"
#include "ui_usersdialog.h"

#include "usersmodel.h"
#include "user.h"

#include <QtGui>
#include <QtSql>

UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersDialog)
{
    ui->setupUi(this);

    if (createConnection())
    {
        initModel();
    }


    ui->btnDelete->setEnabled(m_model->rowCount() > 0);

    connect(ui->txtUser, SIGNAL(returnPressed()), this,
            SLOT(on_btnAdd_clicked()));
}

UsersDialog::~UsersDialog()
{
    delete ui;
}

bool UsersDialog::createConnection()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(qApp->applicationDirPath()+QDir::separator()+"database.s3db");
    return m_db.open();
}

void UsersDialog::initModel()
{
    m_model = new UsersModel(this);
    ui->cmbUsers->setModel(m_model);
}

void UsersDialog::on_btnAdd_clicked()
{
    if (!ui->btnAdd->isEnabled())
        return;

    QString newUser = ui->txtUser->text();

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        if (newUser == m_model->data(m_model->index(i, 0)))
        {
            QMessageBox::information(this,
                                     tr("Ajout d'utilisateur"),
                                     tr("L'utilisateur %1 existe dÃ©jÃ , impossible de l'ajouter Ã  nouveau.")
                                     .arg(newUser));
            return;
        }
    }

    User user;
    user.id = -1;
    user.name = ui->txtUser->text();
    if (m_model->addUser(user))
    {
        ui->btnDelete->setEnabled(m_model->rowCount() > 0);
        ui->txtUser->clear();
        ui->btnAdd->setEnabled(false);
        ui->cmbUsers->setCurrentIndex(ui->cmbUsers->count() - 1);
    }
}

void UsersDialog::on_btnDelete_clicked()
{
    int r = QMessageBox::question(this,
                                  tr("Suppression d'utilisateur"),
                                  tr("Supprimer dÃ©finitivement l'utilisateur %1 ?")
                                  .arg(ui->cmbUsers->currentText()),
                                  QMessageBox::Yes, QMessageBox::Cancel);

    if (r == QMessageBox::Yes)
    {
        m_model->removeRow(ui->cmbUsers->currentIndex());
        ui->btnDelete->setEnabled(m_model->rowCount() > 0);
    }
}

void UsersDialog::on_txtUser_textEdited()
{
    ui->btnAdd->setEnabled(!ui->txtUser->text().isEmpty());
}


void UsersDialog::on_btnStart_clicked()
{
}
