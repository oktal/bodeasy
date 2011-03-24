#include "usersmodel.h"
#include "usersmanager.h"
#include "user.h"

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
            return users[index.row()].name();
       }
   }
   return QVariant();
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : users.count();
}

void UsersModel::removeRow(int row, const QModelIndex &parent)
{
    if (parent.isValid())
    {
        return;
    }

    beginRemoveRows(parent, row, row);
    if (manager->removeUserById(users[row].id()))
    {
        users.removeAt(row);
    }
    endRemoveRows();
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
