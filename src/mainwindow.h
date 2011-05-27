#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}
class QSqlTableModel;
class SessionsModel;
class SessionContentModel;
class SessionFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(qint64 userId, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exercisesAction_triggered();
    void on_sessionsAction_triggered();
    void on_mensurationAction_triggered();
    void on_cmbSessions_currentIndexChanged(int index);
    void on_btnStart_clicked();

private:
    Ui::MainWindow *ui;
    qint64 mUserId;
    SessionsModel *sessionsModel;
    SessionContentModel *contentModel;
    SessionFrame *sessionFrame;

};

#endif // MAINWINDOW_H
