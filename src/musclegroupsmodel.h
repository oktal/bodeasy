#ifndef MUSCLEGROUPMODEL_H
#define MUSCLEGROUPMODEL_H

#include <QAbstractListModel>
#include "musclegroup.h"

class MuscleGroupsManager;

class MuscleGroupsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MuscleGroupsModel(QObject *parent = 0);
    QVariant data( const QModelIndex &index, int role ) const;
    int rowCount( const QModelIndex &parent ) const;

private:
    MuscleGroupsManager *mManager;
    QList<MuscleGroup> mGroups;
};

#endif // MUSCLEGROUPMODEL_H
