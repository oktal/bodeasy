#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class PlanDialog;
}

class SessionsModel;

#include "sql/session.h"

class PlanDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlanDialog(QWidget *parent = 0);
    ~PlanDialog();

    Session session() const;
    QDate date() const;
    
private:
    Ui::PlanDialog *ui;
    SessionsModel *mSessionsModel;
};

#endif // PLANDIALOG_H
