#ifndef EXERCISEWIDGETDATA_H
#define EXERCISEWIDGETDATA_H

#include <QString>
#include <QMetaType>

#include "sql/exercise.h"

struct ExerciseWidgetData
{
    ExerciseWidgetData() : exerciseId(-1), sessionExerciseId(-1), rest(0),
                           weight(false), repetitions(0), series(0)
    {
    }

    qint64 exerciseId;
    qint64 sessionExerciseId;
    QString name;
    Exercise::Type type;
    Exercise::Difficulty difficulty;
    int rest;
    bool weight;
    int repetitions;
    int series;
    QString description;

    int number;
    
    QList<QPair<int, int> > seriesData;
};

typedef QList<ExerciseWidgetData> ExerciseWidgetDataList;

Q_DECLARE_METATYPE( ExerciseWidgetData )

#endif // EXERCISEWIDGETDATA_H
