#include "SessionIconView.h"
#include "SessionIconModel.h"
#include "SessionIconDelegate.h"
#include "SessionProxy.h"

#include <QToolButton>
#include <QDebug>

SessionIconView::SessionIconView( SessionProxy* proxy )
	: QListView( proxy ),
	mProxy( proxy ),
	mModel( new SessionIconModel( this ) ),
	mDelegate( new SessionIconDelegate( this ) )
{
	Q_ASSERT( proxy );
	
	QToolButton* button = new QToolButton( this );
	button->setIcon( QIcon( ":/images/checkround-icon.png" ) );
	
	setUniformItemSizes( true );
	setAlternatingRowColors( false );
	setResizeMode( QListView::Adjust );
	setViewMode( QListView::IconMode );
	setMovement( QListView::Static );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
	setAutoScroll( false );
	setSpacing( 5 );
	setModel( mModel );
	setItemDelegate( mDelegate );
	setCornerWidget( button );
	
	connect( button, SIGNAL( clicked() ), mProxy, SLOT( stop() ) );
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

ExerciseWidgetDataList SessionIconView::widgetsData() const
{
	return mModel->widgetsData();
}

void SessionIconView::setWidgetsData( const ExerciseWidgetDataList& data, bool readOnly )
{
	if ( readOnly ) {
		setEditTriggers( QAbstractItemView::NoEditTriggers );
	}
	else {
		setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked );
	}
	
	mModel->setWidgetsData( data );
}
