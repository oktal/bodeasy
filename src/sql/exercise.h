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
    
    Exercise( const QString& name_ = QString::null,
        Exercise::Type type_ = Exercise::Repetition, Exercise::Difficulty difficulty_ = Exercise::Medium,
        const QList<qint64> &groups_ = QList<qint64>(),
        bool useWeight_ = false, const QString& description_ = QString::null );
    
    qint64 id;
    QString name;
    Exercise::Type type;
    Exercise::Difficulty difficulty;
    QList<qint64> groups;
    bool useWeight;
    QString description;
};

#endif // EXERCICE_H
