#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class PlanDialog;
}

class SessionsModel;

#include "sql/plannedsession.h"

class PlanDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlanDialog(QWidget *parent = 0);
    explicit PlanDialog(const PlannedSession &ps, QWidget *parent = 0);
    ~PlanDialog();

    PlannedSession plannedSession() const;
    
private slots:
    void on_btnToday_clicked();

    void on_cmbTrainings_activated(int index);

    void on_dateEdit_dateChanged(const QDate &date);

    void on_startTimeEdit_timeChanged(const QTime &time);

    void on_endTimeEdit_timeChanged(const QTime &time);

private:
    Ui::PlanDialog *ui;
    SessionsModel *mSessionsModel;
    PlannedSession mPlannedSession;
};

#endif // PLANDIALOG_H
