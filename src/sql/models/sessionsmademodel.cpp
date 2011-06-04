#include "sessionsmademodel.h"
#include "../SqlHelper.h"

#include <QSqlRecord>
#include <QDate>

#include <QDebug>

SessionsMadeModel::SessionsMadeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void SessionsMadeModel::setUserId(qint64 userId)
{
    mUserId = userId;
    setQuery(QString("SELECT s.name, sm.date, sm.id_session, sm.id_session_made FROM session_made sm "
             "INNER JOIN session s "
             "ON s.id_session=sm.id_session "
             "WHERE sm.[id_user]=%1").arg(userId), SqlHelper::database());
}

QVariant SessionsMadeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            const QSqlRecord rec = record(index.row());
            const QDateTime dateTime = rec.value("date").toDateTime();
            const QString sessionName = rec.value("name").toString();

            return QString("%1 - %2").arg(sessionName).arg(dateTime.toString(Qt::SystemLocaleLongDate));
        }
    }

    return QSqlQueryModel::data(index, role);
}
