#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QList>

/**
  \struct Session
  \brief Handle a session represented by the `session` table
  \author m.stefani
*/

struct Session
{
    struct SessionExercise
    {
        SessionExercise(qint64 exerciseId_ = - 1, int series_ = 0, int repetitions_ = 0, int rest_ = 0);

        qint64 exerciseId;
        int series;
        int repetitions;
        int rest;
    };

    Session(const QString &name_ = QString::null, const QString &objective_ = QString::null,
            const QList<SessionExercise> &exercises_ = QList<SessionExercise>());

    qint64 id;
    QString name;
    QString objective;
    QList<SessionExercise> exercises;
};

#endif // SESSION_H
