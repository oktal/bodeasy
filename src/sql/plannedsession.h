#ifndef PLANNEDSESSION_H
#define PLANNEDSESSION_H

#include "session.h"

#include <QDate>
#include <QTime>

struct PlannedSession
{
    PlannedSession();
    qint64 id;
    Session session;
    QDate date;
    QTime startTime;
    QTime endTime;
};

#endif // PLANNEDSESSION_H
