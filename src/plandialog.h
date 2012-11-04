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
    ~PlanDialog();

    PlannedSession plannedSession() const;
    
private slots:
    void on_btnToday_clicked();

private:
    Ui::PlanDialog *ui;
    SessionsModel *mSessionsModel;
};

#endif // PLANDIALOG_H
