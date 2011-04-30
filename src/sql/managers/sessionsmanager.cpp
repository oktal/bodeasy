#include "sessionsmanager.h"
#include "../session.h"

#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

SessionsManager::SessionsManager(QObject *parent) :
    QObject(parent)
{
}

/**
  * \brief Select all sessions
  * \return QList<Session> selected sessions
*/

QList<Session> SessionsManager::selectSessions() const
{
    QSqlQuery query;
    query.prepare("SELECT id_session, name, objective FROM session");

    QList<Session> sessions;
    if (query.exec())
    {
        QSqlQuery exercisesQuery;
        /* Select exercises */
        exercisesQuery.prepare("SELECT id_exercise, series, repetitions, rest "
                               "FROM session_exercise WHERE id_session=:id_session");
        while (query.next())
        {
            exercisesQuery.bindValue(":id_session", query.value(0));
            Session s(query.value(1).toString(),
                      query.value(2).toString());
            s.id = query.value(0).toLongLong();
            if (exercisesQuery.exec())
            {
                while (exercisesQuery.next())
                {
                    s.exercises.append(Session::SessionExercise(
                                           exercisesQuery.value(0).toLongLong(),
                                           exercisesQuery.value(1).toInt(),
                                           exercisesQuery.value(2).toInt(),
                                           exercisesQuery.value(3).toInt()));
                }
            }
            sessions.append(s);
        }
    }
    return sessions;
}

/**
  * \brief Insert a new session
  * \param session the session to insert
      Set the inserted id
  * \return true if the operation succeeded
*/

bool SessionsManager::createSession(Session &session) const
{
    QSqlQuery query;
    query.prepare("INSERT INTO session(name, objective) VALUES(:name, :objective)");
    query.bindValue(":name", session.name);
    query.bindValue(":objective", session.objective);
    if (query.exec())
    {
        session.id = query.lastInsertId().toLongLong();
        /* Insert exercises */
        query.prepare("INSERT INTO session_exercise(id_session, id_exercise, series, repetitions, rest) "
                      "VALUES(:id_session, :id_exercise, :series, :repetitions, :rest)");
        query.bindValue(":id_session", session.id);
        foreach (const Session::SessionExercise &se, session.exercises)
        {
            query.bindValue(":id_exercise", se.exerciseId);
            query.bindValue(":series", se.series);
            query.bindValue(":repetitions", se.repetitions);
            query.bindValue(":rest", se.rest);
            query.exec();
        }
        return true;
    }

    return false;
}

/**
  * \brief Update a session
  * \param session the session to update
  * \return true if the operation succeeded
*/

bool SessionsManager::updateSession(const Session &session) const
{
    QSqlQuery query;
    query.prepare("UPDATE session SET name=:name, objective=:objective "
                  "WHERE id_session=:id_session");
    query.bindValue(":name", session.name);
    query.bindValue(":objective", session.objective);
    query.bindValue(":id_session", session.id);
    if (query.exec())
    {
        /* Update the exercises */
        query.prepare("DELETE FROM session_exercise WHERE id_session=:id_session");
        query.bindValue(":id_session", session.id);
        if (query.exec())
        {
            query.prepare("INSERT INTO session_exercise(id_session, id_exercise, series, repetitions, rest) "
                          "VALUES(:id_session, :id_exercise, :series, :repetitions, :rest)");
            query.bindValue(":id_session", session.id);
            foreach (const Session::SessionExercise &se, session.exercises)
            {
                query.bindValue(":id_exercise", se.exerciseId);
                query.bindValue(":series", se.series);
                query.bindValue(":repetitions", se.repetitions);
                query.bindValue(":rest", se.rest);
                query.exec();
            }
            return true;
        }
    }

    return false;
}

/**
  * \brief Remove a session by the id
  * \param id the session id to remove
  * \return true if the operation succeeded
*/

bool SessionsManager::removeSessionById(qint64 id) const
{
    QSqlQuery query;
    query.prepare("DELETE FROM session WHERE id_session=:id_session");
    query.bindValue(":id_session", id);
    return query.exec();
}
