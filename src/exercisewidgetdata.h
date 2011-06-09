#ifndef EXERCISEWIDGETDATA_H
#define EXERCISEWIDGETDATA_H

#include <QString>
#include <QMetaType>

#include "sql/exercise.h"

struct ExerciseWidgetData
{
    ExerciseWidgetData() : modified(false), exerciseId(-1), sessionExerciseId(-1), rest(0),
                           weight(false), repetitions(0), series(0), number(-1)
    {
    }

    bool modified;
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
    
    typedef QPair<int, int> PairIntInt;
    QList<PairIntInt> seriesData;
};

typedef QList<ExerciseWidgetData> ExerciseWidgetDataList;

Q_DECLARE_METATYPE( ExerciseWidgetData )

#endif // EXERCISEWIDGETDATA_H
