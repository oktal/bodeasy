#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/pChronometer.h"

#include "sessionframe.h"
#include "views/icon/SessionIconView.h"

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"
#ifdef HAVE_KDCHART
#include "graphicsdialog.h"
#endif
#include "settingsdialog.h"
#include "settings.h"
#include "planningwindow.h"
#include "sql/SqlHelper.h"
#include "sql/models/usersmodel.h"
#include "sql/models/sessionsmodel.h"
#include "sql/models/sessioncontentmodel.h"
#include "sql/models/sessionsmademodel.h"
#include "sql/managers/plannedsessionsmanager.h"

#include <QSqlTableModel>

#include <QTimer>
#include <QSettings>

#include <QDebug>
#include <QSqlError>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlRecord>

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

#ifndef HAVE_KDCHART
    ui->graphAction->setVisible( false );
#endif

    setCentralWidget(sessionProxy);
    sessionProxy->setEnabled(false);
    
    /*
    sessionProxy->setWidget(new SessionFrame(sessionProxy));
    sessionProxy->setWidget(new SessionIconView(sessionProxy));
    */

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

    //connect(sessionProxy, SIGNAL(error(const QString&)), this, SLOT(onSessionError(const QString&)));
    connect(sessionProxy, SIGNAL(sessionStarted(SessionProxy::Type,bool)), this, SLOT(onSessionStarted(SessionProxy::Type,bool)));
    //connect(sessionProxy, SIGNAL(sessionCommited(const ExerciseWidgetDataList&)), this, SLOT(onSessionCommited(const ExerciseWidgetDataList&)));
    connect(sessionProxy, SIGNAL(sessionFinished()), this, SLOT(onSessionFinished()));

    readSettings();
    QTimer::singleShot(0, this, SLOT(checkFirstLaunch()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (sessionProxy->isModified())
    {
        const QString text = trUtf8("Une séance non enregistrée est en cours. Que "
                                     "souhaitez-vous faire ?");
        QMessageBox box(this);
        box.setText(text);
        box.setIcon(QMessageBox::Warning);
        QAbstractButton *saveButton = box.addButton(trUtf8("Enregistrer et quitter"), QMessageBox::RejectRole);
        QAbstractButton *closeButton = box.addButton(trUtf8("Quitter sans sauvegarder"), QMessageBox::AcceptRole);
        QAbstractButton *cancelButton = box.addButton(trUtf8("Annuler"), QMessageBox::ApplyRole);
        box.exec();
        QAbstractButton *clickedButton = box.clickedButton();
        if (clickedButton == saveButton)
        {
            sessionProxy->stop(false);
            event->accept();
        }
        else if (clickedButton == closeButton)
        {
            sessionProxy->rollback();
            event->accept();
        }
        else if (clickedButton == cancelButton)
        {
            event->ignore();
            return;
        }
    }
    else
    {
        const QString text = trUtf8("Êtes vous sûr de vouloir quitter '%1'?").arg(qApp->applicationName());

        if (QMessageBox::question(this, QString::null, text, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }
    writeSettings();
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
    ui->btnSee->setEnabled(ui->cmbSessionsMade->currentIndex() != -1);
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

void MainWindow::on_settingsAction_triggered()
{
    SettingsDialog dialog(this);
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

void MainWindow::on_calendarAction_triggered()
{
    PlanningWindow *window = new PlanningWindow(mUserId, this);
    window->show();
}

void MainWindow::on_cmbSessions_activated(int index)
{
    if (sessionProxy->stop())
    {
        const qint64 id = ui->cmbSessions->itemData(index).toLongLong();
        
        contentModel->setSessionId(id);
        sessionProxy->setSessionId(id);
        
        if (index != -1)
        {
            sessionProxy->setRunning(true,SessionProxy::Session,true);
        }
        
        ui->btnStart->setEnabled(index != -1);
    }
    else
    {
        const qint64 sessionId = sessionProxy->sessionId();
        index = ui->cmbSessions->findData( sessionId );
        ui->cmbSessions->setCurrentIndex( index );
    }
}

void MainWindow::on_btnStart_clicked()
{
//    qint64 sessionId = -1;
//    if (ui->radOtherSession->isChecked()) {
//        const int index = ui->cmbSessions->currentIndex();
//        sessionId = sessionsModel->session(index).id;
//    } else {
//        sessionId = mTodaySession.session.id;
//    }

//    sessionProxy->setSessionId(sessionId);
//    contentModel->setSessionId(sessionId);
    sessionProxy->setRunning(true,SessionProxy::Session,false);

}

void MainWindow::on_btnSee_clicked()
{
    const int index = ui->cmbSessionsMade->currentIndex();
    const QSqlRecord record = sessionsMadeModel->record(index);
    const qint64 sessionId = record.value("id_session").toLongLong();
    const qint64 sessionMadeId = record.value("id_session_made").toLongLong();
    
    if (sessionProxy->stop())
    {
        sessionProxy->setSessionId(sessionId);
        contentModel->setSessionId(sessionId);
        sessionProxy->setSessionMadeId(sessionMadeId);
        sessionProxy->setRunning(true,SessionProxy::SessionMade,true);
    }
}

void MainWindow::onSessionUpdated(qint64 id)
{
    const int index = ui->cmbSessions->currentIndex();
    const qint64 currentId = ui->cmbSessions->itemData(index).toLongLong();
    
    if (id == currentId)
    {
        contentModel->update();
        sessionProxy->updateModel();
    }
}

void MainWindow::onSessionDeleted(qint64 id)
{
    if (id == contentModel->sessionId())
    {
        ui->cmbSessions->setCurrentIndex(-1);
        contentModel->setSessionId(-1);
        sessionProxy->setSessionId(-1);
        sessionProxy->rollback();
    }
}

void MainWindow::onSessionStarted(SessionProxy::Type type, bool readOnly)
{
    sessionProxy->setEnabled(!readOnly || type == SessionProxy::SessionMade);
    ui->btnStart->setEnabled(readOnly && ui->cmbSessions->currentIndex() != -1);
}

void MainWindow::onSessionFinished()
{
    sessionProxy->setEnabled(false);
    ui->btnStart->setEnabled(ui->cmbSessions->currentIndex() != -1);
    const int currentSessionMadeIndex = ui->cmbSessionsMade->currentIndex();
    const QSqlQuery query = sessionsMadeModel->query();
    sessionsMadeModel->setQuery(query.executedQuery(), SqlHelper::database());
    ui->cmbSessionsMade->setCurrentIndex( currentSessionMadeIndex );
    ui->lblLastSeanceDate->setText(QDate::currentDate().toString(Qt::SystemLocaleLongDate));
    ui->btnSee->setEnabled(ui->cmbSessionsMade->currentIndex() != -1);
}

void MainWindow::onTimerTimeout()
{
    const QString dt = QDateTime::currentDateTime().toString(
                Qt::SystemLocaleLongDate);
    dateTimeLabel->setText(dt);
}

struct PlannedSessionsSorter {
    bool operator()(const PlannedSession &lhs, const PlannedSession &rhs) {
        return lhs.date < rhs.date;
    }
};

struct TodaySessionFinder {
    bool operator()(const PlannedSession &session) {
        return session.date == QDate::currentDate();
    }
};

struct NextSessionFinder {
    bool operator()(const PlannedSession &session) {
        return session.date > QDate::currentDate();
    }
};

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

     ui->lblPlanifiedSession->setText(trUtf8("Aucune séance planifiée"));
     ui->lblNextSeanceDate->setText(trUtf8("Aucune séance planifiée"));

    PlannedSessionsManager manager;
    QList<PlannedSession> plannedSessions = manager.selectPlannedSessions(mUserId);
    qSort(plannedSessions.begin(), plannedSessions.end(), PlannedSessionsSorter());

    QList<PlannedSession>::const_iterator it = std::find_if(plannedSessions.begin(), plannedSessions.end(),
                                                            TodaySessionFinder());
    const bool isSessionPlanned = it != plannedSessions.end();
    if (isSessionPlanned) {
        mTodaySession = *it;
        sessionProxy->setSessionId(it->session.id);
        contentModel->setSessionId(it->session.id);
        sessionProxy->setRunning(true,SessionProxy::Session,true);
        ui->btnStart->setEnabled(true);
        ui->lblPlanifiedSession->setText(it->session.name);
    }

    QList<PlannedSession>::const_iterator nextSession = std::find_if(plannedSessions.begin(), plannedSessions.end(),
                                                            NextSessionFinder());
    if (nextSession != plannedSessions.end()) {
        ui->lblNextSeanceDate->setText(
                    QString("%1 - %2").arg(nextSession->session.name)
                    .arg(nextSession->date.toString()));
    }

    ui->radPlanifiedSession->setEnabled(isSessionPlanned);
    ui->radPlanifiedSession->setChecked(isSessionPlanned);
    ui->lblPlanifiedSession->setEnabled(isSessionPlanned);
    ui->cmbSessions->setEnabled(!isSessionPlanned);

    if (!isSessionPlanned) {
        sessionProxy->setSessionId(-1);
        contentModel->setSessionId(-1);
        sessionProxy->setRunning(false, SessionProxy::Unknown, false, false);
    }
}

void MainWindow::readSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    const bool restoreWindowState = settings.value(SETTING_WINDOW_SAVESTATE, true).toBool();
    if (restoreWindowState)
    {
        const QByteArray geometry = settings.value(SETTING_WINDOW_GEOMETRY, QByteArray()).toByteArray();
        const QByteArray state = settings.value(SETTING_WINDOW_STATE, QByteArray()).toByteArray();
        if (!geometry.isEmpty())
        {
            restoreGeometry(geometry);
        }
        else
        {
            resize(800, 600);
        }
        if (!state.isEmpty())
        {
            restoreState(state);
        }
    }
    else
    {
        resize(800, 600);
    }




    reloadSettings();
}

void MainWindow::checkFirstLaunch()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    const bool firstLaunch = settings.value(SETTING_FIRSTLAUNCH, true).toBool();
    if (firstLaunch)
    {
        /* Show Mensurations Dialog */
        int const ret = QMessageBox::information(this, tr("Information"),
                                 trUtf8("Vous venez de lancer '%1' pour la première fois. Voulez-vous "
                                        "commencer par saisir vos mensurations ?").arg(qApp->applicationName()),
                                 QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            MensurationsDialog dialog(mUserId);
            dialog.exec();
        }
        settings.setValue(SETTING_FIRSTLAUNCH, false);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    const bool restoreWindowState = settings.value(SETTING_WINDOW_SAVESTATE, true).toBool();
    if (restoreWindowState)
    {
        settings.setValue(SETTING_WINDOW_GEOMETRY, saveGeometry());
        settings.setValue(SETTING_WINDOW_STATE, saveState());
    }
}

void MainWindow::reloadSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    SessionProxy::ViewMode view = static_cast<SessionProxy::ViewMode>(
                settings.value(SETTING_VIEW_MODE, SessionProxy::List).toInt());

    if (sessionProxy->viewMode() == SessionProxy::None)
    {
        sessionProxy->setViewMode(view);
    }
    else if (view != sessionProxy->viewMode())
    {
        sessionProxy->setViewMode(view);
    }

    mStartChrono = settings.value(SETTING_START_CHRONO, true).toBool();
    mResetChrono = settings.value(SETTING_RESET_CHRONO, true).toBool();
    mStopChrono = settings.value(SETTING_STOP_CHRONO, true).toBool();
}

#ifdef HAVE_KDCHART
void MainWindow::on_graphAction_triggered()
{
    GraphicsDialog dialog;
    dialog.exec();
}
#endif


void MainWindow::on_radPlanifiedSession_clicked()
{
    sessionProxy->setSessionId(mTodaySession.session.id);
    contentModel->setSessionId(mTodaySession.session.id);
    sessionProxy->stop(false);
    sessionProxy->setRunning(true, SessionProxy::Session,true);
    ui->btnStart->setEnabled(true);
}
