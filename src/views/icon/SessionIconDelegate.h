#ifndef SESSIONICONDELEGATE_H
#define SESSIONICONDELEGATE_H

#include <QStyledItemDelegate>

class ExerciseWidget;

class SessionIconDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	
public:
	SessionIconDelegate( QObject* parent = 0 );
	virtual ~SessionIconDelegate();
	
	virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
	virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
	virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

protected:
	ExerciseWidget* mWidget;
};

#endif // SESSIONICONDELEGATE_H
