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
    ui->cmbSessions->setCurrentIndex(-1);

    const int sessionsCount = sessionsModel->rowCount();
    ui->lblSeancesCount->setText(sessionsCount == 0 ? trUtf8("Aucune")
                                                    : QString::number(sessionsCount));
    ui->lblPlanifiedSession->setText(trUtf8("Aucune séance planifiée"));
    ui->lblPlanifiedSession->setEnabled(false);

    ui->cChrono->setFormat("hh:mm:ss:zzz");
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
    sessionFrame->setUserId(id);
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
    SessionsDialog dialog(sessionsModel);
    connect(&dialog, SIGNAL(sessionUpdated(qint64)), this, SLOT(onSessionUpdated(qint64)));
    connect(&dialog, SIGNAL(sessionDeleted(qint64)), this, SLOT(onSessionDeleted(qint64)));
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

void MainWindow::on_cmbSessions_activated(int index)
{
    const qint64 id = ui->cmbSessions->itemData(index).toLongLong();
    contentModel->setSessionId(id);
    sessionFrame->setSessionId(id);
    ui->btnStart->setEnabled(true);
}

void MainWindow::on_btnStart_clicked()
{
    sessionFrame->start();
}

void MainWindow::onSessionUpdated(qint64 id)
{
    const int index = ui->cmbSessions->currentIndex();
    const qint64 currentId = ui->cmbSessions->itemData(index).toLongLong();
    if (id == currentId)
    {
        contentModel->update();
        sessionFrame->refresh();
    }
}

void MainWindow::onSessionDeleted(qint64 id)
{
    if (id == contentModel->sessionId())
    {
        ui->cmbSessions->setCurrentIndex(-1);
        contentModel->setSessionId(-1);
        sessionFrame->setSessionId(-1);
    }
}

void MainWindow::selectInformations()
{

}
