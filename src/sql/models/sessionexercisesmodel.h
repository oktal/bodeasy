#ifndef SESSIONEXERCISESMODEL_H
#define SESSIONEXERCISESMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QCache>

#include "../session.h"

class SessionExercisesModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum Columns { Exercise = 0, Series, Repetitions, Rest, Count = Rest + 1 };

    explicit SessionExercisesModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    QList<Session::SessionExercise> sessionExercises() const;
    void setSessionExercises(const QList<Session::SessionExercise> &exercises);

private:
    QList<Session::SessionExercise> mExercises;
    mutable QCache<qint64, QString> mNamesCache;
};

#endif // SESSIONEXERCISESMODEL_H
