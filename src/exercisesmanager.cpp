#include "exercisesmanager.h"
#include "exercise.h"

#include <QtCore>
#include <QtSql>

ExercisesManager::ExercisesManager(QObject *parent) :
    QObject(parent)
{
}

bool ExercisesManager::createExercise(Exercise &exercise)
{
    query.prepare("INSERT INTO exercise(name,"
                    "type,"
                    "difficulty,"
                    "weight,"
                    "description) "
                  "VALUES(:name,"
                    ":type,"
                    ":difficulty,"
                    ":weight,"
                    ":description)");

    query.bindValue(":name", exercise.name);
    query.bindValue(":type", exercise.type);
    query.bindValue(":difficulty", exercise.difficulty);
    query.bindValue(":weight", exercise.useWeight);
    query.bindValue(":description", exercise.description);

    if (query.exec())
    {
        exercise.id = query.lastInsertId().toLongLong();
        return true;
    }
    else {
        qWarning( qPrintable( query.lastError().text() ) );
    }
    return false;
}

QList<Exercise> ExercisesManager::selectExercises()
{
    query.prepare("SELECT id_exercise,"
                    "name,"
                    "type,"
                    "difficulty,"
                    "weight,"
                    "description "
                  "FROM exercise");

    QList<Exercise> exercises;
    if (query.exec())
    {
        while (query.next())
        {
            Exercise ex;
            ex.id = query.value(0).toInt();
            ex.name = query.value(1).toString();
            ex.type = Exercise::Type( query.value(2).toInt() );
            ex.difficulty = Exercise::Difficulty( query.value(3).toInt() );
            ex.useWeight = query.value(4).toBool();
            ex.description = query.value(5).toString();
            exercises.append(ex);
        }
    }

    return exercises;
}

bool ExercisesManager::updateExercise(const Exercise &exercise)
{
    query.prepare("UPDATE exercise "
                  "SET name=:name,"
                    "type=:type,"
                    "difficulty=:difficulty,"
                    "weight=:weight,"
                    "description=:description"
                  "WHERE id_exercise=:id");

    query.bindValue(":name", exercise.name);
    query.bindValue(":type", exercise.type);
    query.bindValue(":difficulty", exercise.difficulty);
    query.bindValue(":weight", exercise.useWeight);
    query.bindValue(":description", exercise.description);
    query.bindValue(":id", exercise.id);
    return query.exec();
}

bool ExercisesManager::removeExerciseById(int id)
{
    query.prepare("DELETE FROM exercise WHERE id_exercise=:id");
    query.bindValue(":id", id);
    return query.exec();
}
