#ifndef SESSIONICONDELEGATE_H
#define SESSIONICONDELEGATE_H

#include <QStyledItemDelegate>

#include "sql/exercise.h"

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
	
	QString typeToString( Exercise::Type type ) const;
	QString difficultyToString( Exercise::Difficulty difficulty ) const;
	void drawFrame( QPainter* painter, const QRect& rect, int radius, const QBrush& pen, const QBrush& brush, const QString& text = QString::null, const QBrush& color = QColor( Qt::black ), int flags = Qt::AlignCenter, const QFont& font = QFont(), int padding = 5 ) const;
	void drawExercise( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

#endif // SESSIONICONDELEGATE_H
