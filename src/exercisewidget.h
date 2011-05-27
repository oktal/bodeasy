#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>

namespace Ui
{
    class ExerciseWidget;
}

class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(qint64 id, QWidget *parent = 0);
    ~ExerciseWidget();
    void setExerciseName(const QString &exerciseName);

private:
    Ui::ExerciseWidget *ui;
    qint64 mSessionExerciseId;
    QString mExerciseName;

};

#endif // EXERCISEWIDGET_H
