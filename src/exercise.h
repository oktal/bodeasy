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

    qint64 id() const;
    QString name() const;
    QString type() const;
    QString difficulty() const;
    bool load() const;
    QString description() const;

    void setId(qint64 id);
    void setName(const QString &name);
    void setType(const QString &type);
    void setDifficulty(const QString &difficulty);
    void setLoad(const bool load);
    void setDescription(const QString &description);

private:
    qint64 id_;
    QString name_;
    QString type_;
    QString difficulty_;
    bool load_;
    QString description_;
};

#endif // EXERCICE_H
