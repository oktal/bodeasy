#include "SessionIconView.h"
#include "SessionIconModel.h"
#include "SessionIconDelegate.h"
#include "exercisewidget.h"
#include "SessionProxy.h"
#include "sql/SqlHelper.h"

#include <QScrollBar>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

SessionIconView::SessionIconView( QWidget* parent )
	: QListView( parent ),
	mModel( new SessionIconModel( this ) ),
	mDelegate( new SessionIconDelegate( this ) )
{
	setUniformItemSizes( true );
	setAlternatingRowColors( false );
	setResizeMode( QListView::Adjust );
	setViewMode( QListView::IconMode );
	setMovement( QListView::Static );
	setSpacing( 5 );
	setModel( mModel );
	setItemDelegate( mDelegate );
}

SessionIconView::~SessionIconView()
{
}

QSize SessionIconView::minimumSizeHint() const
{
	return sizeHint();
}

QSize SessionIconView::sizeHint() const
{
	return
		mDelegate->sizeHint( QStyleOptionViewItem(), QModelIndex() )
		+QSize( style()->pixelMetric( QStyle::PM_ScrollBarExtent ), 0 )
		+QSize( spacing() *2, 0 )
		;
}

SessionProxy* SessionIconView::sessionProxy() const
{
	return qobject_cast<SessionProxy*>( parentWidget() );
}

void SessionIconView::sessionUpdated( const ExerciseWidgetDataList& data, bool readOnly )
{
	if ( readOnly ) {
		setEditTriggers( QAbstractItemView::NoEditTriggers );
	}
	else {
		setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked );
	}
	
	mModel->setWidgetsData( data );
}

ExerciseWidgetDataList SessionIconView::sessionData() const
{
	return mModel->widgetsData();
}
