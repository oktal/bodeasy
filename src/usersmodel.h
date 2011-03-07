#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "user.h"

class UsersManager;

class UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UsersModel(QObject *parent = 0);
    bool addUser(User &user);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void removeRow(int row, const QModelIndex &parent = QModelIndex());

signals:

public slots:

private:
    UsersManager *manager;
    QList<User> users;

};

#endif // USERSMODEL_H
