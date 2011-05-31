#include "SessionIconDelegate.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QDebug>

SessionIconDelegate::SessionIconDelegate( SessionIconView* parent )
	: QStyledItemDelegate( parent ),
	mView( parent )
{
	Q_ASSERT( mView );
}

SessionIconDelegate::~SessionIconDelegate()
{
}

QWidget* SessionIconDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.column() == 0 ) {
		ExerciseWidget* ew = mView->mExercises.value( index.data().toLongLong() );
		
		if ( ew ) {
			ew->setParent( parent );
		}
		
		return ew;
	}
	
	return QStyledItemDelegate::createEditor( parent, option, index );
}

void SessionIconDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	ExerciseWidget* ew = mView->mExercises.value( index.data().toLongLong() );
	
	if ( !ew ) {
		QStyledItemDelegate::paint( painter, option, index );
	}
	else {
		painter->drawPixmap( option.rect.topLeft(), QPixmap::grabWidget( ew ) );
	}
}

QSize SessionIconDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	ExerciseWidget* ew = mView->mExercises.value( index.data().toLongLong() );
	return ew ? ew->size() : QStyledItemDelegate::sizeHint( option, index );
}
