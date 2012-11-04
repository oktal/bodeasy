#ifndef PLANNEDSESSIONSMANAGER_H
#define PLANNEDSESSIONSMANAGER_H

#include <QObject>
#include <QList>

#include "../plannedsession.h"

class PlannedSessionsManager : public QObject
{
    Q_OBJECT
public:
    explicit PlannedSessionsManager(QObject *parent = 0);
    bool createPlannedSession(PlannedSession &session) const;
    QList<PlannedSession> selectPlannedSessions() const;
    bool updatePlannedSession(const PlannedSession &session) const;
    bool removePlannedSessionById(qint64 id) const;
    
};

#endif // PLANNEDSESSIONSMANAGER_H
