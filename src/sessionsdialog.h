#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include <QDialog>

namespace Ui
{
    class SessionsDialog;
}

class SessionsModel;
class SessionExercisesModel;

class SessionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionsDialog(SessionsModel *model, QWidget *parent = 0);
    ~SessionsDialog();

signals:
    void sessionUpdated(qint64 id);
    void sessionDeleted(qint64 id);

private slots:
    void on_btnAdd_clicked();
    void on_btnModify_clicked();
    void on_btnDelete_clicked();
    void on_btnReset_clicked();
    void on_btnAddExercise_clicked();
    void on_btnDeleteExercise_clicked();

    void on_lstSessions_clicked();
    void on_txtSessionName_textEdited(const QString &text);

private:
    Ui::SessionsDialog *ui;
    SessionsModel *sessionsModel;
    SessionExercisesModel *sessionExercisesModel;
};

#endif // SESSIONSDIALOG_H
