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
qWarning( "ici" );
    query.prepare("INSERT INTO Exercise(name,"
                    "type,"
                    "difficulty,"
                    "useWeight,"
                    "description) "
                  "VALUES(:name,"
                    ":type,"
                    ":difficulty,"
                    ":useWeight,"
                    ":description)");

    query.bindValue(":name", exercise.name);
    query.bindValue(":type", exercise.type);
    query.bindValue(":difficulty", exercise.difficulty);
    query.bindValue(":useWeight", exercise.useWeight);
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
    query.prepare("SELECT id_Exercise,"
                    "name,"
                    "type,"
                    "difficulty,"
                    "useWeight,"
                    "description "
                  "FROM Exercise");

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
    query.prepare("UPDATE Exercise "
                  "SET name=:name,"
                    "type=:type,"
                    "difficulty=:difficulty,"
                    "useWeight=:useWeight,"
                    "description=:description"
                  "WHERE id_Exercise=:id");

    query.bindValue(":name", exercise.name);
    query.bindValue(":type", exercise.type);
    query.bindValue(":difficulty", exercise.difficulty);
    query.bindValue(":useWeight", exercise.useWeight);
    query.bindValue(":description", exercise.description);
    query.bindValue(":id", exercise.id);
    return query.exec();
}

bool ExercisesManager::removeExerciseById(int id)
{
    query.prepare("DELETE FROM Exercise WHERE id_Exercise=:id");
    query.bindValue(":id", id);
    return query.exec();
}
