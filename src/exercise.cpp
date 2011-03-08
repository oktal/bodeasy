#include "exercise.h"

Exercise::Exercise()
{
    id_ = -1;
    name_ = QString::null;
    type_ = QString::null;
    difficulty_ = QString::null;
    load_ = false;
    description_ = QString::null;
}

Exercise::Exercise(qint64 id,
    const QString &name,
    const QString &type,
    const QString &difficulty,
    const bool load,
    const QString &description)
{
    id_ = id;
    name_ = name;
    type_ = type;
    difficulty_ = difficulty;
    load_ = load;
    description_ = description;
}

QString Exercise::description() const
{
    return description_;
}

QString Exercise::difficulty() const
{
    return difficulty_;
}

qint64 Exercise::id() const
{
    return id_;
}

bool Exercise::load() const
{
    return load_;
}

QString Exercise::name() const
{
    return name_;
}

QString Exercise::type() const
{
    return type_;
}

void Exercise::setDescription(const QString &description)
{
    description_ = description;
}

void Exercise::setDifficulty(const QString &difficulty)
{
    difficulty_ = difficulty;
}

void Exercise::setId(qint64 id)
{
    id_ = id;
}

void Exercise::setLoad(const bool load)
{
    load_ = load;
}

void Exercise::setName(const QString &name)
{
    name_ = name;
}

void Exercise::setType(const QString &type)
{
    type_ = type;
}
