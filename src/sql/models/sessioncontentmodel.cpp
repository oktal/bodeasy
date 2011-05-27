#include "sessioncontentmodel.h"
#include "../SqlHelper.h"

#include <QDebug>
#include <QSqlError>
#include <QString>
#include <QStringList>

static QString stringForDifficulty(Exercise::Difficulty difficulty)
{
    switch (difficulty)
    {
    case Exercise::Easy:
        return QObject::trUtf8("Facile");
        break;
    case Exercise::Medium:
        return QObject::trUtf8("Moyen");
        break;
    case Exercise::Hard:
        return QObject::trUtf8("Difficile");
        break;
    }

    return QString::null;
}

static QString muscleGroupNameForId(qint64 id)
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT name FROM muscle_group WHERE id_muscle_group=:id");
    query.bindValue(":id", id);
    if (query.exec() && query.next())
    {
        return query.value(0).toString();
    }

    return "";
}

SessionContentModel::SessionContentModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int SessionContentModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : mExercises.count();
}

QVariant SessionContentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= mExercises.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
        return mExercises[index.row()].name;
        break;
    case Qt::ToolTipRole:
        return formatToolTip(index);
        break;
    }

    return QVariant();
}

void SessionContentModel::setSessionId(qint64 id)
{
    mSessionId = id;
    beginResetModel();
    mExercises.clear();
    selectExercises();
    endResetModel();
}

void SessionContentModel::selectExercises()
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT id_exercise FROM session_exercise WHERE id_session=:sessionId");
    query.bindValue(":sessionId", mSessionId);
    if (query.exec())
    {
        while (query.next())
        {
            QSqlQuery exerciseQuery = SqlHelper::query();
            exerciseQuery.prepare("SELECT name, type, difficulty, weight, description "
                                   "FROM exercise WHERE id_exercise=:exerciseId");
            exerciseQuery.bindValue(":exerciseId", query.value(0));
            if (exerciseQuery.exec())
            {
                while (exerciseQuery.next())
                {
                    Exercise ex(
                      exerciseQuery.value(0).toString(),
                      Exercise::Type(exerciseQuery.value(1).toInt()),
                      Exercise::Difficulty(exerciseQuery.value(2).toInt()),
                      QList<qint64>(),
                      exerciseQuery.value(3).toBool(),
                      exerciseQuery.value(4).toString());

                    QSqlQuery muscleGroupsQuery = SqlHelper::query();
                    muscleGroupsQuery.prepare("SELECT id_muscle_group FROM exercise_muscle_group "
                                              "WHERE id_exercise=:exerciseId");
                    muscleGroupsQuery.bindValue(":exerciseId", query.value(0));
                    if (muscleGroupsQuery.exec())
                    {
                        while (muscleGroupsQuery.next())
                        {
                            ex.groups.append(muscleGroupsQuery.value(0).toLongLong());
                        }
                    }

                    mExercises.append(ex);
                }
            }
            else
            {
                qDebug() << query.lastError();
            }
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
}

QString SessionContentModel::formatToolTip(const QModelIndex &index) const
{
    const Exercise &e = mExercises[index.row()];
    const QString type = e.type == Exercise::Repetition ? trUtf8("Répétition") :
                                                          trUtf8("Durée");
    const QString difficulty = stringForDifficulty(e.difficulty);
    const QString useWeight = e.useWeight ? trUtf8("Oui") : trUtf8("Non");

    QStringList groups;
    foreach(qint64 id, e.groups)
    {
        if (!mGroupsCache.contains(id))
        {
            mGroupsCache.insert(id, new QString(muscleGroupNameForId(id)));
        }

        groups << *mGroupsCache.object(id);
    }

    return trUtf8("<strong>Groupes musculaires</strong>: %1<br/>"
                  "<strong>Type</strong>: %2<br />"
                  "<strong>Difficulté</strong>: %3<br />"
                  "<strong>Poids</strong>: %4<br />"
                  "<strong>Description</strong>: %5")
            .arg(groups.join(", "))
            .arg(type).arg(difficulty).arg(useWeight).arg(e.description);
}
