#ifndef EXERCISEWIDGETDATA_H
#define EXERCISEWIDGETDATA_H

#include <QString>
#include <QMetaType>

#include "sql/exercise.h"

struct ExerciseWidgetData
{
    ExerciseWidgetData() : exerciseId(-1), rest(0), weight(false), repetitions(0), series(0)
    {
    }

    qint64 exerciseId;
    QString name;
    Exercise::Type type;
    Exercise::Difficulty difficulty;
    int rest;
    bool weight;
    int repetitions;
    int series;
};

typedef QList<ExerciseWidgetData> ExerciseWidgetDataList;

Q_DECLARE_METATYPE( ExerciseWidgetData )

#endif // EXERCISEWIDGETDATA_H
