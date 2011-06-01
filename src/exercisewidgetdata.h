#ifndef EXERCISEWIDGETDATA_H
#define EXERCISEWIDGETDATA_H

#include <QString>
#include "sql/exercise.h"

struct ExerciseWidgetData
{
    ExerciseWidgetData() : rest(0), weight(false), repetitions(0), series(0)
    {
    }

    QString name;
    Exercise::Type type;
    Exercise::Difficulty difficulty;
    int rest;
    bool weight;
    int repetitions;
    int series;
};

#endif // EXERCISEWIDGETDATA_H
