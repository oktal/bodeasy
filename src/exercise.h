#ifndef EXERCICE_H
#define EXERCICE_H

#include <QString>

class Exercise
{
public:
    Exercise();
    Exercise(qint64 id,
        const QString &name,
        const QString &type,
        const QString &difficulty,
        const bool load,
        const QString &description = QString::null);

    QString description() const;
    QString difficulty() const;
    qint64 id() const;
    bool load() const;
    QString name() const;
    QString type() const;

    void setDescription(const QString &description);
    void setDifficulty(const QString &difficulty);
    void setId(qint64 id);
    void setLoad(const bool load);
    void setName(const QString &name);
    void setType(const QString &type);

private:
    QString description_;
    QString difficulty_;
    qint64 id_;
    bool load_;
    QString name_;
    QString type_;
};

#endif // EXERCICE_H
