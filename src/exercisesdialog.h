#ifndef EXERCISESDIALOG_H
#define EXERCISESDIALOG_H

#include <QDialog>

namespace Ui
{
    class ExercisesDialog;
}

class ExercisesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExercisesDialog(QWidget *parent = 0);
    ~ExercisesDialog();

private:
    Ui::ExercisesDialog *ui;
};

#endif // EXERCISESDIALOG_H
