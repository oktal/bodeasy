#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/pChronometer.h"

#include "SessionProxy.h"
#include "sessionframe.h"
#include "views/icon/SessionIconView.h"

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"
#include "sql/SqlHelper.h"
#include "sql/models/usersmodel.h"
#include "sql/models/sessionsmodel.h"
#include "sql/models/sessioncontentmodel.h"
#include "sql/models/sessionsmademodel.h"

#include <QSqlTableModel>

#include <QTimer>

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
    sessionsMadeModel(new SessionsMadeModel(this)),
    sessionProxy(new SessionProxy(this)),
    cChrono(new pChronometer(this)),
    dateTimeLabel(new QLabel(this))
{
    ui->setupUi(this);
    sessionProxy->setEnabled(false);
    setCentralWidget(sessionProxy);
    
    sessionProxy->setWidget(new SessionFrame(this));
    sessionProxy->setWidget(new SessionIconView(this));

    ui->cmbSessions->setModel(sessionsModel);
    ui->lstContent->setModel(contentModel);
    ui->cmbSessions->setCurrentIndex(-1);

    const int sessionsCount = sessionsModel->rowCount();
    ui->lblSeancesCount->setText(sessionsCount == 0 ? trUtf8("Aucune")
                                                    : QString::number(sessionsCount));
    ui->lblPlanifiedSession->setText(trUtf8("Aucune séance planifiée"));
    ui->lblPlanifiedSession->setEnabled(false);

    ui->cmbSessionsMade->setModel(sessionsMadeModel);

    cChrono->setTextFormat("hh:mm:ss:zzz");
    cChrono->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QFont myFont("Monospace", 10, QFont::Bold);
    dateTimeLabel->setFont(myFont);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(dateTimeLabel);
    ui->toolBar->addWidget(cChrono);
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    onTimerTimeout();

    connect(sessionProxy, SIGNAL(sessionFinished()), this, SLOT(onSessionFinished()));
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
    sessionProxy->setUserId(id);
    sessionsMadeModel->setUserId(id);
    ui->btnSee->setEnabled(sessionsMadeModel->rowCount() > 0);
    UsersModel users;
    setWindowTitle(QString("%1 (%2)").arg(qApp->applicationName()).arg(users.user(id).name));

    selectInformations();
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
    sessionProxy->setSessionId(id);
    ui->btnStart->setEnabled(true);
}

void MainWindow::on_btnStart_clicked()
{
    sessionProxy->setEnabled(true);
    sessionProxy->start();
}

void MainWindow::onSessionUpdated(qint64 id)
{
    const int index = ui->cmbSessions->currentIndex();
    const qint64 currentId = ui->cmbSessions->itemData(index).toLongLong();
    if (id == currentId)
    {
        contentModel->update();
        sessionProxy->refresh();
    }
}

void MainWindow::onSessionDeleted(qint64 id)
{
    if (id == contentModel->sessionId())
    {
        ui->cmbSessions->setCurrentIndex(-1);
        contentModel->setSessionId(-1);
        sessionProxy->setSessionId(-1);
    }
}

void MainWindow::onSessionFinished()
{
    sessionProxy->setEnabled(false);
    const QSqlQuery query = sessionsMadeModel->query();
    sessionsMadeModel->setQuery(query.executedQuery(), SqlHelper::database());
    ui->lblLastSeanceDate->setText(QDate::currentDate().toString(Qt::SystemLocaleLongDate));
}

void MainWindow::onTimerTimeout()
{
    const QString dt = QDateTime::currentDateTime().toString(
                Qt::SystemLocaleLongDate);
    dateTimeLabel->setText(dt);
}

void MainWindow::selectInformations()
{
    QSqlQuery q = SqlHelper::query();
    q.prepare("SELECT date FROM session_made WHERE id_user=:userId "
              "ORDER BY date DESC LIMIT 1");
    q.bindValue(":userId", mUserId);
    if (q.exec())
    {
        if (q.next())
        {
            QDate lastSeanceDate = q.value(0).toDate();
            ui->lblLastSeanceDate->setText(lastSeanceDate.toString(Qt::SystemLocaleLongDate));
        }
        else
        {
            ui->lblLastSeanceDate->setText(trUtf8("Aucune séance effectuée"));
        }
    }

    ui->lblNextSeanceDate->setText(trUtf8("Aucune séance planifiée"));
}
