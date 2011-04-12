#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>


class UsersModel;
namespace Ui
{
    class UsersDialog;
}

class UsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsersDialog(QWidget *parent = 0);
    ~UsersDialog();

private:

    Ui::UsersDialog *ui;
    QSqlDatabase m_db;
    UsersModel *m_model;

    bool createConnection();
    void initModel();

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_txtUser_textEdited();

    void on_btnStart_clicked();

};

#endif // USERSDIALOG_H
