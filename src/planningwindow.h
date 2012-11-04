#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "sql/plannedsession.h"
#include "sql/user.h"

namespace Ui {
class PlanningWindow;
}

class BasicCalendarModel;
class BasicCalendarItem;
class PlannedSessionsManager;

class PlanningWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlanningWindow(qint64 userId, QWidget *parent = 0);
    ~PlanningWindow();
    
private slots:
    void on_planifyAction_triggered();
    void onItemClicked(const QDate &date, int row);

    void on_removeAction_triggered();

    void on_editAction_triggered();

private:
    Ui::PlanningWindow *ui;
    qint64 mUserId;
    BasicCalendarModel *mCalendarModel;
    PlannedSessionsManager *mManager;

    QMap<BasicCalendarItem *, PlannedSession> mPlannedSessions;
    BasicCalendarItem *mCurrentItem;
};

#endif // PLANNINGWINDOW_H
