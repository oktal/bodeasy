#ifndef SESSIONICONDELEGATE_H
#define SESSIONICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>

#include "sql/exercise.h"

class QAbstractItemView;
class ExerciseWidget;

class SessionIconDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	
public:
	SessionIconDelegate( QAbstractItemView* parent );
	virtual ~SessionIconDelegate();
	
	virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
	virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
	virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	
	void clearCachedEditors( QAbstractItemModel* model ) const;

protected:
	QAbstractItemView* mView;
	ExerciseWidget* mEditor;
	
	QString cachedEditorKey( const QModelIndex& index, const QStyleOptionViewItem& option ) const;
	QPixmap cachedEditor( const QModelIndex& index, const QStyleOptionViewItem& option ) const;
	void invalidateCachedEditor( const QModelIndex& index ) const;

protected slots:
	void application_focusChanged( QWidget* old, QWidget* now );
};

#endif // SESSIONICONDELEGATE_H
