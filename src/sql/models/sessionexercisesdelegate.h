#ifndef SESSIONEXERCISESDELEGATE_H
#define SESSIONEXERCISESDELEGATE_H

#include <QStyledItemDelegate>

class ExercisesModel;

class SessionExercisesDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SessionExercisesDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    ExercisesModel *exercisesModel;

};

#endif // SESSIONEXERCISESDELEGATE_H
