#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>
#include <QScopedPointer>
#include <QSqlDatabase>

namespace Ui
{
    class UsersDialog;
}

class QSqlTableModel;

class UsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsersDialog(QWidget *parent = 0);
    ~UsersDialog();

private:

    QScopedPointer<Ui::UsersDialog> ui;
    QSqlDatabase m_db;
    QSqlTableModel *m_model;

    bool createConnection();
    void initModel();

private slots:
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_txtUser_textEdited();

    void on_btnStart_clicked();

};

#endif // USERSDIALOG_H
