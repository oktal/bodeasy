#include "plannedsessionsmanager.h"

#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include "sql/SqlHelper.h"

PlannedSessionsManager::PlannedSessionsManager(QObject *parent) :
    QObject(parent)
{
}

bool PlannedSessionsManager::createPlannedSession(PlannedSession &session) const
{
    QSqlQuery query = SqlHelper::query();

    const bool transaction = SqlHelper::transaction();

    query.prepare("INSERT INTO session_planned(id_session, date, start_time, end_time) "
                  "VALUES(:id_session, :date, :start_time, :end_time)");
    query.bindValue(":id_session", session.session.id);
    query.bindValue(":date", session.date);
    query.bindValue(":start_time", session.startTime);
    query.bindValue(":end_time", session.endTime);

    if (query.exec()) {
        session.id = query.lastInsertId().toInt();

        if (transaction) {
            SqlHelper::commit();
            return true;
        }

        return false;
    }

    else if (transaction) {
        SqlHelper::rollback();
    }

    return false;
}

QList<PlannedSession> PlannedSessionsManager::selectPlannedSessions() const
{
    QList<PlannedSession> plannedSessions;
    QSqlQuery query = SqlHelper::query();

    query.prepare("SELECT s.id_session, s.name, s.objective, sp.id_session_planned, sp.date, sp.start_time, sp.end_time "
                  "FROM session_planned sp INNER JOIN session s ON sp.id_session=s.id_session");
    if (query.exec()) {
        while (query.next()) {
            PlannedSession ps;
            Session session;
            session.id = query.value(0).toInt();
            session.name = query.value(1).toString();
            session.objective = query.value(2).toString();

            ps.id = query.value(3).toInt();
            ps.session = session;
            ps.date = query.value(4).toDate();
            ps.startTime = query.value(5).toTime();
            ps.endTime = query.value(6).toTime();

            plannedSessions << ps;
        }
    }

    return plannedSessions;
}

bool PlannedSessionsManager::updatePlannedSession(const PlannedSession &session) const
{
    const bool transaction = SqlHelper::transaction();

    QSqlQuery query = SqlHelper::query();
    query.prepare("UPDATE session_planned SET "
                  "date=:date, start_time=:startTime, end_time=:endTime "
                  "WHERE id_session_planned=:sessionId");
    query.bindValue(":date", session.date);
    query.bindValue(":startTime", session.startTime);
    query.bindValue(":endTime", session.endTime);
    query.bindValue(":sessionId", session.id);

    if (query.exec()) {
        if (transaction) {
            SqlHelper::commit();
            return true;
        }

        return false;
    }

    else if (transaction) {
        qDebug() << query.lastError();
        SqlHelper::rollback();
    }

    return false;
}

bool PlannedSessionsManager::removePlannedSessionById(qint64 id) const
{
    const bool transaction = SqlHelper::transaction();

    QSqlQuery query = SqlHelper::query();
    query.prepare("DELETE FROM session_planned WHERE id_session_planned=:id");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (transaction) {
            SqlHelper::commit();
            return true;
        }

        return false;
    }

    else if (transaction) {
        SqlHelper::rollback();
    }

    return false;
}
