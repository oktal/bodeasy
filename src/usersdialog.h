#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

class UsersModel;
namespace Ui {
    class UsersDialog;
}

class UsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsersDialog(QWidget *parent = 0);
    virtual ~UsersDialog();

private:
    Ui::UsersDialog *ui;
    UsersModel *m_model;

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_txtUser_textEdited();

    virtual void accept();

};

#endif // USERSDIALOG_H
