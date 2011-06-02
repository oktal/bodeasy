#include "user.h"

bool User::operator==(const User &other) const
{
    return id == other.id;
}

/*
User::User() : id_(-1), name_(QString::null)
{
}

User::User(qint64 id, const QString &name) :
    id_(id), name_(name)
{
}

qint64 User::id() const
{
    return id_;
}

QString User::name() const
{
    return name_;
}

void User::setId(qint64 id)
{
    id_ = id;
}

void User::setName(const QString &name)
{
    name_ = name;
}
*/
