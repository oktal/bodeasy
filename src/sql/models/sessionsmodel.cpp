#include "sessionsmodel.h"
#include "../managers/sessionsmanager.h"

#include <QAbstractTableModel>
#include <QStringList>

SessionsModel::SessionsModel(QObject *parent) :
    QAbstractTableModel(parent),
    mManager(new SessionsManager(this))
{
    mSessions = mManager->selectSessions();
}

QModelIndex SessionsModel::index(const Session &session) const
{
    for ( int i = 0; i < mSessions.count(); i++ ) {
        const Session& s = mSessions[ i ];

        if ( s.name.compare( session.name, Qt::CaseInsensitive ) == 0 ) {
            return QAbstractTableModel::index( i, 0 );
        }
     }

    return QModelIndex();
}

Session SessionsModel::session(const QModelIndex &index) const
{
    if (index.row() < 0 || index.row() >= mSessions.count())
    {
        return Session();
    }

    return mSessions[index.row()];
}

bool SessionsModel::addSession(Session &session)
{
    if (mManager->createSession(session))
    {
        beginInsertRows(QModelIndex(), mSessions.count(), mSessions.count());
        mSessions.append(session);
        endInsertRows();
        return true;
    }

    return false;
}

bool SessionsModel::updateSession(const Session &session)
{
    if (mManager->updateSession(session))
    {
        for (int i = 0; i < mSessions.count(); ++i)
        {
            if (mSessions[i].id == session.id)
            {
                mSessions[i] = session;
                break;
            }
        }
        emit dataChanged(QModelIndex(), QModelIndex());
        return true;
    }

    return false;
}

bool SessionsModel::removeRow(int row, const QModelIndex &parent)
{
    if (row < 0 || row > mSessions.count())
    {
        return false;
    }

    if (mManager->removeSessionById(mSessions[row].id))
    {
        beginRemoveRows(parent, row, row);
        mSessions.removeAt(row);
        endRemoveRows();
        return true;
    }

    return false;
}

int SessionsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mSessions.count();
}

int SessionsModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant SessionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case SessionName:
            return trUtf8("Nom de la session");
            break;
        case ExercisesCount:
            return trUtf8("Exercices");
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant SessionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > mSessions.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
        if (index.column() == 0)
        {
            return mSessions[index.row()].name;
        }
        else
        {
            return mSessions[index.row()].exercises.count();
        }
        break;
    case Qt::UserRole:
        return mSessions[index.row()].id;
        break;
    }

    return QVariant();
}
