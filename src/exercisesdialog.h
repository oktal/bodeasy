#ifndef EXERCISESDIALOG_H
#define EXERCISESDIALOG_H

#include <QDialog>

namespace Ui
{
    class ExercisesDialog;
}

class ExercisesModel;

class ExercisesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExercisesDialog(QWidget *parent = 0);
    ~ExercisesDialog();

protected slots:
    void on_btnAdd_clicked();
    void on_btnModify_clicked();
    void on_btnDelete_clicked();

private:
    Ui::ExercisesDialog *ui;
    ExercisesModel* exercisesModel;
};

#endif // EXERCISESDIALOG_H
