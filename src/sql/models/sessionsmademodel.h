#ifndef SESSIONSMADEMODEL_H
#define SESSIONSMADEMODEL_H

#include <QSqlTableModel>

class SessionsMadeModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SessionsMadeModel(QObject *parent = 0);
    void setUserId(qint64 userId);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    qint64 mUserId;

};

#endif // SESSIONSMADEMODEL_H
