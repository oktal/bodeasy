#include "musclegroupsmanager.h"
#include "../musclegroup.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

MuscleGroupsManager::MuscleGroupsManager(QObject *parent) :
    QObject(parent)
{
}

QList<MuscleGroup> MuscleGroupsManager::selectMuscleGroups()
{
    query.prepare("SELECT id_muscle_group, name FROM muscle_group");
    QList<MuscleGroup> groups;
    if (query.exec())
    {
        while (query.next())
        {
            MuscleGroup group;
            group.id = query.value(0).toLongLong();
            group.name = query.value(1).toString();
            groups.append(group);
        }
    }

    return groups;
}
