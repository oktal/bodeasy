#ifndef SESSIONSMODEL_H
#define SESSIONSMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "../session.h"

class SessionsManager;

class SessionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { SessionName = 0, ExercisesCount, Count = ExercisesCount + 1 };

    explicit SessionsModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    Session session(const QModelIndex &index) const;
    bool addSession(Session &session);
    bool updateSession(const Session &session);

private:
    SessionsManager *mManager;
    QList<Session> mSessions;
};

#endif // SESSIONSMODEL_H
