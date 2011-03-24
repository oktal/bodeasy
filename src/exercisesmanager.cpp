#include "exercisesmanager.h"
#include "exercise.h"

#include <QtCore>
#include <QtSql>

ExercisesManager::ExercisesManager(QObject *parent) :
    QObject(parent)
{
}

bool ExercisesManager::createExercise(Exercise &Exercise)
{
    query.prepare("INSERT INTO Exercise(name"
                    "type,"
                    "difficulty,"
                    "load,"
                    "description) "
                  "VALUES(:name,"
                    ":type,"
                    ":difficulty,"
                    ":load,"
                    ":description)");

    query.bindValue(":name", Exercise.name());
    query.bindValue(":type", Exercise.type());
    query.bindValue(":difficulty", Exercise.difficulty());
    query.bindValue(":load", Exercise.load());
    query.bindValue(":description", Exercise.description());

    if (query.exec())
    {
        Exercise.setId(query.lastInsertId().toLongLong());
        return true;
    }
    return false;
}

QList<Exercise> ExercisesManager::selectExercises()
{
    query.prepare("SELECT id_Exercise,"
                    "name,"
                    "type,"
                    "difficulty,"
                    "load,"
                    "description "
                  "FROM Exercise");

    QList<Exercise> Exercises;
    if (query.exec())
    {
        while (query.next())
        {
            Exercises.append(Exercise(query.value(0).toInt(),
                              query.value(1).toString(),
                              query.value(2).toString(),
                              query.value(3).toString(),
                              query.value(4).toBool(),
                              query.value(5).toString()));
        }
    }

    return Exercises;
}

bool ExercisesManager::updateExercise(const Exercise &Exercise)
{
    query.prepare("UPDATE Exercise "
                  "SET name=:name,"
                    "type=:type,"
                    "difficulty=:difficulty,"
                    "load=:load,"
                    "description=:description"
                  "WHERE id_Exercise=:id");

    query.bindValue(":name", Exercise.name());
    query.bindValue(":type", Exercise.type());
    query.bindValue(":difficulty", Exercise.difficulty());
    query.bindValue(":load", Exercise.load());
    query.bindValue(":description", Exercise.description());
    query.bindValue(":id", Exercise.id());
    return query.exec();
}

bool ExercisesManager::removeExerciseById(int id)
{
    query.prepare("DELETE FROM Exercise WHERE id_Exercise=:id");
    query.bindValue(":id", id);
    return query.exec();
}
