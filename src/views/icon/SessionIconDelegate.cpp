#include "SessionIconDelegate.h"
#include "SessionIconModel.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QDebug>

SessionIconDelegate::SessionIconDelegate( QObject* parent )
	: QStyledItemDelegate( parent ),
	mWidget( new ExerciseWidget )
{
}

SessionIconDelegate::~SessionIconDelegate()
{
	delete mWidget;
}

QWidget* SessionIconDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.column() == 0 ) {
		ExerciseWidget* editor = new ExerciseWidget( parent );
		editor->setFocusPolicy( Qt::StrongFocus );
		return editor;
	}
	
	return QStyledItemDelegate::createEditor( parent, option, index );
}

void SessionIconDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.isValid() ) {
		const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
		mWidget->setData( data );
		painter->drawPixmap( option.rect.topLeft(), QPixmap::grabWidget( mWidget ) );
	}
	else {
		QStyledItemDelegate::paint( painter, option, index );
	}
}

void SessionIconDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
	const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
	ExerciseWidget* ed = qobject_cast<ExerciseWidget*>( editor );
	
	if ( ed ) {
		ed->setData( data );
	}
	else {
		QStyledItemDelegate::setEditorData( editor, index );
	}
}

void SessionIconDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
	ExerciseWidget* ed = qobject_cast<ExerciseWidget*>( editor );
	
	if ( ed ) {
		model->setData( index, QVariant::fromValue( ed->data() ), Qt::EditRole );
	}
	else {
		QStyledItemDelegate::setModelData( editor, model, index );
	}
}

QSize SessionIconDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return index.isValid() ? mWidget->size() : QStyledItemDelegate::sizeHint( option, index );
}
