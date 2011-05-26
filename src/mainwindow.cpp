#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionframe.h"

#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"
#include "sql/SqlHelper.h"
#include "sql/models/sessionsmodel.h"
#include "sql/models/sessioncontentmodel.h"

#include <QSqlTableModel>

#include <QDebug>
#include <QSqlError>

MainWindow::MainWindow(qint64 userId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUserId(userId),
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

