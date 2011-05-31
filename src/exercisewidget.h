#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPair>

#include "sql/exercise.h"

namespace Ui
{
    class ExerciseWidget;
}

class QLineEdit;

class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(qint64 id, QWidget *parent = 0);
    ~ExerciseWidget();
    void setExerciseName(const QString &exerciseName);
    void setExerciseType(Exercise::Type type);
    void setExerciseDifficulty(Exercise::Difficulty difficulty);
    void setExerciseRest(int rest);
    void setExerciseUseWeight(bool weight);
    void setExerciseRepetitions(int repetitions);
    void setExerciseSeries(int series);

    bool save(qint64 userId, qint64 sessionId);
    bool isComplete() const;

protected:
    void changeEvent(QEvent *);

private:
    Ui::ExerciseWidget *ui;
    qint64 mExerciseId;
    QString mExerciseName;
    bool mUseWeight;

    QList<QPair<QLineEdit *, QLineEdit *> > mPairs;

};

#endif // EXERCISEWIDGET_H
