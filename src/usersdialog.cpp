#include "usersdialog.h"
#include "ui_usersdialog.h"

#include <QtGui>
#include <QtSql>

UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(txtUser, SIGNAL(textEdited(const QString &)),
            this, SLOT(on_txtUser_textEdited()));

    if (createConnection())
    {
        initModel();
        loadCmbUsers();
    }

    btnDelete->setEnabled(m_model->rowCount() > 0);
}

UsersDialog::~UsersDialog()
{

}

bool UsersDialog::createConnection()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(qApp->applicationDirPath()+QDir::separator()+"database.s3db");
    return m_db.open();
}

void UsersDialog::initModel()
{
    m_model = new QSqlTableModel(this);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setTable("user");
    m_model->select();
}

void UsersDialog::loadCmbUsers()
{
    cmbUsers->setModel(m_model);
    cmbUsers->setModelColumn(m_model->fieldIndex("name"));
}

void UsersDialog::on_btnAdd_clicked()
{
    QString newUser = txtUser->text();

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        if (newUser == m_model->data(m_model->index(i, m_model->fieldIndex("name"))))
        {
            QMessageBox::information(this,
                                     "Ajout d'utilisateur",
                                     "L'utilisateur "+newUser+" existe déjà, impossible de l'ajouter à nouveau.");
            return;
        }
    }

    int rowCount = m_model->rowCount();
    QSqlRecord rec = m_model->record();

    rec.setNull("id_user");
    rec.setValue("name", txtUser->text());
    m_model->insertRecord(rowCount, rec);
    m_model->submit();
    btnDelete->setEnabled(m_model->rowCount() > 0);
}

void UsersDialog::on_btnDelete_clicked()
{
    int r = QMessageBox::question(this,
                                  "Suppression d'utilisateur",
                                  "Supprimer définitivement l'utilisateur "+cmbUsers->currentText()+"?",
                                  QMessageBox::Yes, QMessageBox::Cancel);

    if (r == QMessageBox::Yes)
    {
        m_model->removeRow(cmbUsers->currentIndex());
        m_model->submit();
        btnDelete->setEnabled(m_model->rowCount() > 0);
    }
    else if (r == QMessageBox::Cancel)
    {
        return;
    }

    m_model->removeRow(cmbUsers->currentIndex());
    m_model->submit();
    btnDelete->setEnabled(m_model->rowCount() > 0);
}

void UsersDialog::on_txtUser_textEdited()
{
    btnAdd->setEnabled(!txtUser->text().isEmpty());
}

