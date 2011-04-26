#include "musclegroupsmodel.h"
#include "musclegroupsmanager.h"

#include <QAbstractListModel>

#include <QDebug>

MuscleGroupsModel::MuscleGroupsModel(QObject *parent) :
    QAbstractListModel( parent )
{
    mManager = new MuscleGroupsManager(this);
    mGroups = mManager->selectMuscleGroups();
}

QVariant MuscleGroupsModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() > mGroups.count() )
    {
        return QVariant();
    }

    switch ( role )
    {
    case Qt::DisplayRole:
        return mGroups[ index.row() ].name;
        break;
    case Qt::UserRole:
        return mGroups[ index.row() ].id;
        break;
    }

    return QVariant();
}

int MuscleGroupsModel::rowCount( const QModelIndex &parent ) const
{
    return parent.isValid() ? 0 : mGroups.count();
}
