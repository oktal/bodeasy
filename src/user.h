#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    User(qint64 id, const QString &name);

    qint64 id() const;
    QString name() const;

    void setId(qint64 id);
    void setName(const QString &name);

private:
    qint64 id_;
    QString name_;
};

#endif // USER_H
