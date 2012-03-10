#include "usersmanager.h"
#include "sql/SqlHelper.h"
#include "sql/user.h"

#include <QVariant>

UsersManager::UsersManager(QObject *parent) :
    QObject(parent), query(SqlHelper::query())
{
}

bool UsersManager::createUser(User &user)
{
    query.prepare("INSERT INTO user(name) VALUES(:name)");
    query.bindValue(":name", user.name);
    if (query.exec())
    {
        user.id = query.lastInsertId().toLongLong();
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
            User user;
            user.id = query.value(0).toInt();
            user.name = query.value(1).toString();
            users.append(user);
        }
    }

    return users;
}

bool UsersManager::updateUser(const User &user)
{
    query.prepare("UPDATE user "
                  "SET name=:name "
                  "WHERE id_user=:id");
    query.bindValue(":name", user.name);
    query.bindValue(":id", user.id);
    return query.exec();
}

bool UsersManager::removeUserById(int id)
{
    query.prepare("DELETE FROM user WHERE id_user=:id");
    query.bindValue(":id", id);
    return query.exec();
}
