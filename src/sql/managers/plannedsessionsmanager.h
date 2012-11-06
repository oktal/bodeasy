#ifndef PLANNEDSESSIONSMANAGER_H
#define PLANNEDSESSIONSMANAGER_H

#include <QObject>
#include <QList>

#include "../plannedsession.h"
#include "../user.h"

class PlannedSessionsManager : public QObject
{
    Q_OBJECT
public:
    explicit PlannedSessionsManager(QObject *parent = 0);
    bool createPlannedSession(PlannedSession &session, qint64 userId) const;
    QList<PlannedSession> selectPlannedSessions(qint64 userId) const;
    bool updatePlannedSession(const PlannedSession &session) const;
    bool removePlannedSessionById(qint64 id) const;
    
};

#endif // PLANNEDSESSIONSMANAGER_H
