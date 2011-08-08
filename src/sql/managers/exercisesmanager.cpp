#include "exercisesmanager.h"
#include "sql/exercise.h"
#include "sql/SqlHelper.h"

#include <QtCore>
#include <QtSql>

ExercisesManager::ExercisesManager(QObject *parent) :
    QObject(parent)
{
}

bool ExercisesManager::createExercise(Exercise &exercise) const
{
    QSqlQuery query = SqlHelper::query();
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
        query.prepare("INSERT INTO exercise_muscle_group(id_exercise, id_muscle_group) "
                      "VALUES(:id_exercise, :id_muscle_group)");
        query.bindValue(":id_exercise", exercise.id);
        foreach (qint64 group, exercise.groups)
        {
            query.bindValue(":id_muscle_group", group);
            query.exec();
        }
        return true;
    }
    else {
        qWarning( "%s", qPrintable( query.lastError().text() ) );
    }
    return false;
}

QList<Exercise> ExercisesManager::selectExercises() const
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT e.id_exercise, mg.id_muscle_group, e.name, e.type, e.weight, "
                  "e.difficulty, e.description FROM exercise e "
                  "INNER JOIN exercise_muscle_group mg ON e.id_exercise = mg.id_exercise");
    QList<Exercise> exercises;
    if (query.exec())
    {
        if (query.next())
        {
            Exercise ex(query.value(2).toString(),
                        Exercise::Type(query.value(3).toInt()),
                        Exercise::Difficulty(query.value(5).toInt()),
                        QList<qint64>(),
                        query.value(4).toBool(),
                        query.value(6).toString());
            ex.id = query.value(0).toLongLong();
            ex.groups.append(query.value(1).toLongLong());
            while (query.next())
            {
                if (query.value(0).toLongLong() == ex.id)
                {
                    ex.groups.append(query.value(1).toLongLong());
                }
                else
                {
                    exercises.append(ex);
                    ex.id = query.value(0).toLongLong();
                    ex.name = query.value(2).toString();
                    ex.type = Exercise::Type(query.value(3).toInt());
                    ex.useWeight = query.value(4).toBool();
                    ex.difficulty = Exercise::Difficulty(query.value(5).toInt());
                    ex.description = query.value(6).toString();
                    ex.groups.clear();
                    ex.groups.append(query.value(1).toLongLong());
                }
            }
            exercises.append(ex);
        }
    }
    /*
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
        QSqlQuery groupsQuery;
        while (query.next())
        {
            Exercise ex;
            ex.id = query.value(0).toInt();
            ex.name = query.value(1).toString();
            ex.type = Exercise::Type( query.value(2).toInt() );
            ex.difficulty = Exercise::Difficulty( query.value(3).toInt() );
            ex.useWeight = query.value(4).toBool();
            ex.description = query.value(5).toString();

            groupsQuery.prepare("SELECT id_muscle_group FROM exercise_muscle_group "
                                "WHERE id_exercise=:id_exercise");
            groupsQuery.bindValue(":id_exercise", ex.id);
            if (groupsQuery.exec())
            {
                while (groupsQuery.next())
                {
                    ex.groups.append(groupsQuery.value(0).toLongLong());
                }
            }
            exercises.append(ex);
        }
    }
    */

    return exercises;
}

bool ExercisesManager::updateExercise(const Exercise &exercise) const
{    
    QSqlQuery query = SqlHelper::query();
    query.prepare("UPDATE exercise "
                  "SET name=:name,"
                    "type=:type,"
                    "difficulty=:difficulty,"
                    "weight=:weight,"
                    "description=:description "
                    "WHERE id_exercise=:id");

    query.bindValue(":name", exercise.name);
    query.bindValue(":type", exercise.type);
    query.bindValue(":difficulty", exercise.difficulty);
    query.bindValue(":weight", exercise.useWeight);
    query.bindValue(":description", exercise.description);
    query.bindValue(":id", exercise.id);
    if (query.exec())
    {
        QSqlQuery groupsQuery = SqlHelper::query();

        groupsQuery.prepare("DELETE FROM exercise_muscle_group "
                            "WHERE id_exercise=:id_exercise");
        groupsQuery.bindValue(":id_exercise", exercise.id);
        if (groupsQuery.exec())
        {
            groupsQuery.prepare("INSERT INTO exercise_muscle_group(id_exercise, id_muscle_group) "
                                "VALUES(:id_exercise, :id_muscle_group)");
            groupsQuery.bindValue(":id_exercise", exercise.id);
            foreach (qint64 group, exercise.groups)
            {
                groupsQuery.bindValue(":id_muscle_group", group);
                groupsQuery.exec();
            }
        }

        return true;
    }

    return false;

}

bool ExercisesManager::removeExerciseById(int id) const
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("DELETE FROM exercise WHERE id_exercise=:id");
    query.bindValue(":id", id);
    return query.exec();
}
