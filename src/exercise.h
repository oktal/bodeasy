#ifndef EXERCICE_H
#define EXERCICE_H

#include <QString>
#include <QVector>

struct Exercise
{
    qint64 id;
    QString name;
    QString type;
    QString difficulty;
    QVector<QString> groups;
    bool load;
    QString description;
};

#endif // EXERCICE_H
