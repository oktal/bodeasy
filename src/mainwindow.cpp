#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionframe.h"

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"
#include "sql/SqlHelper.h"
#include "sql/models/usersmodel.h"
#include "sql/models/sessionsmodel.h"
#include "sql/models/sessioncontentmodel.h"

#include <QSqlTableModel>

#include <QDebug>
#include <QSqlError>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUserId(-1),
    sessionsModel(new SessionsModel(this)),
    contentModel(new SessionContentModel(this)),
    sessionFrame(new SessionFrame)
{
    ui->setupUi(this);
    sessionFrame->setEnabled(false);
    ui->mainLayout->addWidget(sessionFrame, 0, 1, 1, 2);

    ui->cmbSessions->setModel(sessionsModel);
    ui->lstContent->setModel(contentModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    const QString text = trUtf8( "Êtes vous sûr de vouloir quitter '%1'?" ).arg(qApp->applicationName());
    
    if (QMessageBox::question(this, QString::null, text, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    {
        event->ignore();
        return;
    }
    
    QMainWindow::closeEvent(event);
}

qint64 MainWindow::userId() const
{
    return mUserId;
}

void MainWindow::setUserId(qint64 id)
{
    mUserId = id;
    UsersModel users;
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(users.user(id).name));
}

void MainWindow::on_exercisesAction_triggered()
{
    ExercisesDialog dialog;
    dialog.exec();
}

void MainWindow::on_sessionsAction_triggered()
{
    SessionsDialog dialog;
    dialog.exec();
}

void MainWindow::on_mensurationAction_triggered()
{
    MensurationsDialog dialog(mUserId);
    dialog.exec();
}

void MainWindow::on_userAction_triggered()
{
    UsersDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        setUserId(dialog.userId());
    }
}

void MainWindow::on_cmbSessions_currentIndexChanged(int index)
{
    const qint64 id = ui->cmbSessions->itemData(index).toLongLong();
    contentModel->setSessionId(id);
    sessionFrame->setSessionId(id);
}

void MainWindow::on_btnStart_clicked()
{
    sessionFrame->start();
}

