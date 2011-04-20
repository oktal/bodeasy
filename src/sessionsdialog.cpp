#include "sessionsdialog.h"

SessionsDialog::SessionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void SessionsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
