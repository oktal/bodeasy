#ifndef MUSCLEGROUPMANAGER_H
#define MUSCLEGROUPMANAGER_H

#include <QObject>
#include <QList>
#include <QSqlQuery>

#include "musclegroup.h"

class MuscleGroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit MuscleGroupsManager(QObject *parent = 0);
    QList<MuscleGroup> selectMuscleGroups();

private:
    QSqlQuery query;
};

#endif // MUSCLEGROUPMANAGER_H
