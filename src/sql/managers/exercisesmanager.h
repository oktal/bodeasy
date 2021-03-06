#ifndef EXERCISESMANAGER_H
#define EXERCISESMANAGER_H

#include <QObject>
#include <QList>
#include <QSqlQuery>

#include "../exercise.h"

class ExercisesManager : public QObject
{
    Q_OBJECT
public:
    explicit ExercisesManager(QObject *parent = 0);
    bool createExercise(Exercise &exercise) const;
    QList<Exercise> selectExercises() const;
    bool updateExercise(const Exercise &exercise) const;
    bool removeExerciseById(int id) const;

};

#endif // EXERCISESMANAGER_H
