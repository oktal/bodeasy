#ifndef EXERCICE_H
#define EXERCICE_H

#include <QString>
#include <QList>

struct Exercise
{
    enum Type {
        Repetition = 0,
        Duration = 1
    };
    
    enum Difficulty {
        Easy = 0,
        Medium = 1,
        Hard = 2
    };
    
    Exercise( const QString& name = QString::null,
        Exercise::Type type = Exercise::Repetition, Exercise::Difficulty difficulty = Exercise::Medium,
        bool useWeight = false, const QString& description = QString::null );
    
    qint64 id;
    QString name;
    Exercise::Type type;
    Exercise::Difficulty difficulty;
    QList<qint64> groups;
    bool useWeight;
    QString description;
};

#endif // EXERCICE_H
