#include "exercise.h"

Exercise::Exercise( const QString& name_, Exercise::Type type_, Exercise::Difficulty difficulty_,
                    const QList<qint64> &groups_, bool useWeight_, const QString& description_ ) :
    id( -1 ),
    name( name_ ),
    type( type_ ),
    difficulty( difficulty_ ),
    groups( groups_ ),
    useWeight( useWeight_ ),
    description( description_ )
{
}
