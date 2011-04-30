#include "sessionexercisesmodel.h"

#include <QDebug>
#include <QList>
#include <QSqlQuery>
#include <QCache>

/**
  * \brief return the exercise's name associated to the
  id \a id
*/

static QString exerciseNameForId(qint64 id)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM exercise WHERE id_exercise=:id");
    query.bindValue(":id", id);
    if (query.exec() && query.next())
    {
        return query.value(0).toString();
    }
    return QString::null;
}

SessionExercisesModel::SessionExercisesModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QList<Session::SessionExercise> SessionExercisesModel::sessionExercises() const
{
    return mExercises;
}

void SessionExercisesModel::setSessionExercises(const QList<Session::SessionExercise> &exercises)
{
    beginResetModel();
    mExercises.clear();
    mExercises = exercises;
    endResetModel();
}

Qt::ItemFlags SessionExercisesModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

int SessionExercisesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mExercises.count();
}

int SessionExercisesModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : Count;
}

bool SessionExercisesModel::insertRow(int row, const QModelIndex &parent)
{
    if (row < 0 || row > mExercises.count())
    {
        return false;
    }

    beginInsertRows(parent, row, row);
    mExercises.insert(row, Session::SessionExercise(0, 0, 0, 0));
    endInsertRows();
    return true;
}

bool SessionExercisesModel::removeRow(int row, const QModelIndex &parent)
{
    if (row < 0 || row > mExercises.count())
    {
        return false;
    }

    beginRemoveRows(parent, row, row);
    mExercises.removeAt(row);
    endRemoveRows();
    return true;
}

QVariant SessionExercisesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > mExercises.count())
    {
        return QVariant();
    }

    if (index.column() == Exercise)
    {
        qint64 id = mExercises[index.row()].exerciseId;
        switch (role)
        {
        case Qt::DisplayRole:
            if (!mNamesCache.contains(id))
            {
                QString name = exerciseNameForId(id);
                mNamesCache.insert(id, new QString(name));
            }
            return *mNamesCache.object(id);
            break;
        case Qt::EditRole:
            return id;
            break;
        }
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column())
        {
        case Series:
            return mExercises[index.row()].series;
            break;
        case Repetitions:
             return mExercises[index.row()].repetitions;
             break;
        case Rest:
            return mExercises[index.row()].rest;
            break;
        }
        break;
    case Qt::TextAlignmentRole:
        return index.column() == Exercise ? Qt::AlignVCenter : Qt::AlignCenter;
        break;
    }

    return QVariant();
}

QVariant SessionExercisesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Exercise:
            return trUtf8("Exercice");
            break;
        case Series:
            return trUtf8("Séries");
            break;
        case Repetitions:
            return trUtf8("Répétitions");
            break;
        case Rest:
            return trUtf8("Repos");
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool SessionExercisesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() > mExercises.count())
    {
        return false;
    }

    switch (role)
    {
    case Qt::EditRole:
        switch (index.column())
        {
        case Exercise:
            mExercises[index.row()].exerciseId = value.toLongLong();
            break;
        case Series:
            mExercises[index.row()].series = value.toInt();
            break;
        case Repetitions:
            mExercises[index.row()].repetitions = value.toInt();
            break;
        case Rest:
            mExercises[index.row()].rest = value.toInt();
            break;
        }
        emit dataChanged(index, index);
    }

    return true;
}
