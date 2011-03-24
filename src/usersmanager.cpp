#include "usersmanager.h"
#include "user.h"

#include <QtCore>
#include <QtSql>

UsersManager::UsersManager(QObject *parent) :
    QObject(parent)
{
}

bool UsersManager::createUser(User &user)
{
    query.prepare("INSERT INTO user(name) VALUES(:name)");
    query.bindValue(":name", user.name());
    if (query.exec())
    {
        user.setId(query.lastInsertId().toLongLong());
        return true;
    }
    return false;
}

QList<User> UsersManager::selectUsers()
{
    query.prepare("SELECT id_user, name FROM user");
    QList<User> users;
    if (query.exec())
    {
        while (query.next())
        {
            users.append(User(query.value(0).toInt(),
                              query.value(1).toString()));
        }
    }

    return users;
}

bool UsersManager::updateUser(const User &user)
{
    query.prepare("UPDATE user "
                  "SET name=:name "
                  "WHERE id_user=:id");
    query.bindValue(":name", user.name());
    query.bindValue(":id", user.id());
    return query.exec();
}

bool UsersManager::removeUserById(int id)
{
    query.prepare("DELETE FROM user WHERE id_user=:id");
    query.bindValue(":id", id);
    return query.exec();
}
