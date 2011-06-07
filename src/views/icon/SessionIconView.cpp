#include "SessionIconView.h"
#include "SessionIconModel.h"
#include "SessionIconDelegate.h"
#include "exercisewidget.h"
#include "SessionProxy.h"
#include "sql/SqlHelper.h"

#include <QScrollBar>
#include <QSqlQuery>
#include <QMessageBox>
#include <QToolButton>
#include <QDate>
#include <QDebug>

SessionIconView::SessionIconView( QWidget* parent )
	: QListView( parent ),
	mModel( new SessionIconModel( this ) ),
	mDelegate( new SessionIconDelegate( this ) )
{
	QToolButton* button = new QToolButton( this );
	button->setIcon( QIcon( ":/images/checkround-icon.png" ) );
	
	setUniformItemSizes( true );
	setAlternatingRowColors( false );
	setResizeMode( QListView::Adjust );
	setViewMode( QListView::IconMode );
	setMovement( QListView::Static );
	setSpacing( 5 );
	setModel( mModel );
	setItemDelegate( mDelegate );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	setCornerWidget( button );
	
	connect( button, SIGNAL( clicked() ), this, SLOT( commitSession() ) );
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

void SessionIconView::commitSession( bool askUser )
{
	if ( askUser ) {
		const QString text = tr( "Save data?" );
		
		if ( QMessageBox::question( this, QString::null, text, QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No ) {
			emit finishSession();
			return;
		}
	}
	
	emit commitSession( mModel->widgetsData() );
	emit finishSession();
}
