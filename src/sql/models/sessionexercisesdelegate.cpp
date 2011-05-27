#include "sessionexercisesdelegate.h"
#include "sessionexercisesmodel.h"

#include "exercisesmodel.h"

#include <QWidget>
#include <QComboBox>
#include <QPainter>

#include <QDebug>

SessionExercisesDelegate::SessionExercisesDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    exercisesModel(new ExercisesModel(this))
{
}

QWidget *SessionExercisesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(option);
    if (index.column() == SessionExercisesModel::Exercise)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setModel(exercisesModel);
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void SessionExercisesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    if (index.column() == SessionExercisesModel::Exercise)
    {
        QComboBox *comboExercises = qobject_cast<QComboBox *>(editor);
        if (comboExercises)
        {
            model->setData(index, comboExercises->itemData(comboExercises->currentIndex()));
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void SessionExercisesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == SessionExercisesModel::Exercise)
    {
        QVariant id = index.model()->data(index, Qt::EditRole);
        QComboBox *comboExercises = qobject_cast<QComboBox *>(editor);
        if (id.isValid() && comboExercises)
        {
            int comboIndex = comboExercises->findData(id);
            comboExercises->setCurrentIndex(comboIndex == -1 ? 0 : comboIndex);
        }
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void SessionExercisesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    if (index.column() == SessionExercisesModel::Rest)
    {
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(option.palette.highlightedText().color());
        }

        QTextOption opt;
        opt.setAlignment(Qt::AlignmentFlag(index.model()->data(index, Qt::TextAlignmentRole).toInt()));
        painter->drawText(option.rect, tr("%1 secs.").arg(
                              index.model()->data(index).toInt()), opt);
        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
