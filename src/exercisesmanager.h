#ifndef EXERCISESMANAGER_H
#define EXERCISESMANAGER_H

#include <QObject>
#include <QList>
#include <QSqlQuery>

#include "exercise.h"

class ExercisesManager : public QObject
{
    Q_OBJECT
public:
    explicit ExercisesManager(QObject *parent = 0);
    bool createExercise(Exercise &exercise);
    QList<Exercise> selectExercises();
    bool updateExercise(const Exercise &exercise);
    bool removeExerciseById(int id);

signals:

public slots:

private:
    QSqlQuery query;

};

#endif // EXERCISESMANAGER_H
