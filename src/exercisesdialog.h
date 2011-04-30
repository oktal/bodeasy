#ifndef EXERCISESDIALOG_H
#define EXERCISESDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "sql/exercise.h"

namespace Ui
{
    class ExercisesDialog;
}

class ExercisesModel;
class MuscleGroupsModel;

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
    void on_lstExercises_clicked( const QModelIndex& index );

    void on_chkGroup2_clicked( bool checked );
    void on_chkGroup3_clicked( bool checked );

    void on_leName_textEdited( const QString &text );

private:
    QList<qint64> groups() const;

    Ui::ExercisesDialog *ui;
    ExercisesModel *exercisesModel;
    MuscleGroupsModel *groupsModel;

    qint64 mCurrentId;
};

#endif // EXERCISESDIALOG_H
