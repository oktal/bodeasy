#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H

#include <QObject>
#include <QList>

#include "../session.h"

/**
  \class SessionsManager
  \brief Session's Data Object Access
      Manage all sessions sql queries
  \author m.stefani
*/

class SessionsManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionsManager(QObject *parent = 0);
    QList<Session> selectSessions() const;
    bool createSession(Session &session) const;
    bool updateSession(const Session &session) const;
    bool removeSessionById(qint64 id) const;

};

#endif // SESSIONSMANAGER_H
