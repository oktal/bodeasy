#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include "ui_sessionsdialog.h"

class SessionsDialog : public QDialog, private Ui::SessionsDialog
{
    Q_OBJECT

public:
    explicit SessionsDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // SESSIONSDIALOG_H
