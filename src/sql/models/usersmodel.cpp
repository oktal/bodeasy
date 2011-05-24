#include "usersmodel.h"
#include "../managers/usersmanager.h"

#include <QAbstractListModel>
#include <QDebug>

UsersModel::UsersModel(QObject *parent) :
    QAbstractListModel(parent)
{
    manager = new UsersManager(this);
    users = manager->selectUsers();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole)
   {
       if (index.row() < users.count())
       {
            return users[index.row()].name;
       }
   }
   return QVariant();
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : users.count();
}

bool UsersModel::addUser(User &user)
{
    if (manager->createUser(user))
    {
        beginInsertRows(QModelIndex(), users.count(), users.count());
        users.append(user);
        endInsertRows();
        return true;
    }


    return false;
}

void UsersModel::removeUser(const User &user)
{
    const int row = users.indexOf(user);
    
    if (row != -1)
    {
        if (manager->removeUserById(user.id))
        {
            beginRemoveRows(QModelIndex(), row, row);
            users.removeAt(row);
            endRemoveRows();
        }
    }
}

User UsersModel::user(const QModelIndex &index) const
{
    return users.value( index.row() );
}

QModelIndex UsersModel::userIndex(const User &user, int column) const
{
    const int row = users.indexOf(user);
    return row != -1 ? QAbstractListModel::index(row, column, QModelIndex()) : QModelIndex();
}
