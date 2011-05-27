#include "session.h"

Session::Session(const QString &name_, const QString &objective_,
                 const QList<SessionExercise> &exercises_) :
    name(name_), objective(objective_), exercises(exercises_)
{

}

Session::SessionExercise::SessionExercise(qint64 exerciseId_, int series_, int repetitions_, int rest_) :
   exerciseId(exerciseId_), series(series_), repetitions(repetitions_), rest(rest_)
{

}
