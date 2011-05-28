#ifndef SESSIONCONTENTMODEL_H
#define SESSIONCONTENTMODEL_H

#include <QAbstractListModel>
#include <QCache>
#include "../exercise.h"

class SessionContentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SessionContentModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setSessionId(qint64 id);
    
    void update();

private:
    QList<Exercise> mExercises;
    qint64 mSessionId;
    mutable QCache<qint64, QString> mGroupsCache;

    void selectExercises();
    QString formatToolTip(const QModelIndex &index) const;

};

#endif // SESSIONCONTENTMODEL_H
