#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "../user.h"

class UsersManager;

class UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UsersModel(QObject *parent = 0);
    bool addUser(User &user);
    void removeUser(const User &user);
    User user(const QModelIndex &index) const;
    User user(qint64 id) const;
    QModelIndex userIndex(const User &user, int column = 0) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

signals:

public slots:

private:
    UsersManager *manager;
    QList<User> users;

};

#endif // USERSMODEL_H
