#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "SessionProxy.h"
#include "settingsdialog.h"

#include "sql/plannedsession.h"

namespace Ui
{
    class MainWindow;
}
class QSqlTableModel;
class QLabel;
class QTimer;
class SessionsModel;
class SessionContentModel;
class SessionsMadeModel;
class pChronometer;

class MainWindow : public QMainWindow
{

friend class SettingsDialog;
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    qint64 userId() const;
    void setUserId(qint64 id);

private slots:
    void on_exercisesAction_triggered();
    void on_sessionsAction_triggered();
    void on_mensurationAction_triggered();
    void on_userAction_triggered();
    void on_settingsAction_triggered();
    void on_cmbSessions_activated(int index);
    void on_btnStart_clicked();
    void on_btnSee_clicked();

    void onSessionUpdated(qint64 id);
    void onSessionDeleted(qint64 id);
    void onSessionStarted(SessionProxy::Type type, bool readOnly);
    void onSessionFinished();

    void onTimerTimeout();
    void checkFirstLaunch();
#ifdef HAVE_KDCHART
    void on_graphAction_triggered();
#endif

    void on_calendarAction_triggered();

    void on_radPlanifiedSession_clicked();

private:
    void selectInformations();
    void readSettings();
    void writeSettings();
    void reloadSettings();

    Ui::MainWindow *ui;
    qint64 mUserId;
    SessionsModel *sessionsModel;
    SessionContentModel *contentModel;
    SessionsMadeModel *sessionsMadeModel;
    SessionProxy* sessionProxy;
    pChronometer* cChrono;
    QLabel *dateTimeLabel;
    QTimer *timer;

    bool mStartChrono;
    bool mResetChrono;
    bool mStopChrono;

    PlannedSession mTodaySession;

protected slots:
    virtual void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
