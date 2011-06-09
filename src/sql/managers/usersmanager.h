#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include <QObject>
#include <QList>
#include <QSqlQuery>

#include "../user.h"

class UsersManager : public QObject
{
    Q_OBJECT
public:
    explicit UsersManager(QObject *parent = 0);
    bool createUser(User &user);
    QList<User> selectUsers();
    bool updateUser(const User &user);
    bool removeUserById(int id);

signals:

public slots:

private:
    QSqlQuery query;

};

#endif // USERSMANAGER_H
