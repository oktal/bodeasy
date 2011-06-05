#ifndef SESSIONICONDELEGATE_H
#define SESSIONICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>

#include "sql/exercise.h"

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
	QTextDocument mDocument;
	
	QString exerciseWidgetTr( const QString& string ) const;
	QString typeToString( Exercise::Type type ) const;
	QString difficultyToString( Exercise::Difficulty difficulty ) const;
	
	void drawFakeLabel( QPainter* painter, const QStyleOptionFrameV3& option, const QString& text, Qt::Alignment align = Qt::AlignCenter, Qt::TextFormat format = Qt::AutoText, QStyle* style = 0, QWidget* widget = 0 ) const;
	void drawExercise( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

#endif // SESSIONICONDELEGATE_H
