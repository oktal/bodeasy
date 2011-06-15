#include "SessionIconView.h"
#include "SessionIconModel.h"
#include "SessionIconDelegate.h"
#include "SessionProxy.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

SessionIconView::SessionIconView( SessionProxy* proxy )
	: QListView( proxy ),
	mProxy( proxy ),
	mModel( new SessionIconModel( this ) ),
	mDelegate( new SessionIconDelegate( this ) ),
	mSeparator( new QFrame( this ) ),
	mControl( new SessionControlWidget( this ) )
{
	Q_ASSERT( proxy );
	
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
	
	mSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setContentsMargins( 0, 0, style()->pixelMetric( QStyle::PM_ScrollBarExtent ), 0 );
	vl->setSpacing( 0 );
	vl->addStretch();
	vl->addWidget( mSeparator );
	vl->addWidget( mControl );
	
	mControl->installEventFilter( this );
	
	connect( mControl, SIGNAL( buttonClicked( SessionControlWidget::Button ) ), this, SLOT( buttonClicked( SessionControlWidget::Button ) ) );
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

bool SessionIconView::objectiveDone() const
{
    return mControl->isObjectiveChecked();
}

void SessionIconView::setWidgetsData( const ExerciseWidgetDataList& data, const QString& objective, bool objectiveDone, bool readOnly )
{
	if ( readOnly ) {
		setEditTriggers( QAbstractItemView::NoEditTriggers );
	}
	else {
		setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked );
	}
	
	mModel->setWidgetsData( data );
	
	mControl->setObjectiveText( objective );
    mControl->setObjectiveChecked( objectiveDone );
    mControl->setObjectiveReadOnly( readOnly );
    mControl->setButtonEnabled( SessionControlWidget::FinishButton, !readOnly );
	
	selectionChanged( QItemSelection(), QItemSelection() );
}

bool SessionIconView::eventFilter( QObject* o, QEvent* e )
{
	if ( o == mControl ) {
		if ( e->type() == QEvent::Resize ) {
			setViewportMargins( 0, 0, 0, mSeparator->size().height() +mControl->size().height() );
		}
	}
	
	return QListView::eventFilter( o, e );
}

void SessionIconView::selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	QListView::selectionChanged( selected, deselected );
	const QModelIndex index = selectedIndexes().value( 0 );
	const int count = model()->rowCount();
	
	mControl->setButtonEnabled( SessionControlWidget::PreviousButton, index.isValid() && index.row() > 0 );
	mControl->setButtonEnabled( SessionControlWidget::NextButton, index.isValid() && index.row() < count -1 );
	mControl->setButtonEnabled( SessionControlWidget::FirstButton, index.isValid() && index.row() > 0 );
	mControl->setButtonEnabled( SessionControlWidget::LastButton, index.isValid() && index.row() < count -1 );
}

void SessionIconView::buttonClicked( SessionControlWidget::Button button )
{
	QModelIndex index = selectionModel()->hasSelection() ? selectedIndexes().value( 0 ) : model()->index( 0, 0 );
	
	switch ( button ) {
		case SessionControlWidget::FinishButton:
			mProxy->stop();
			return;
		case SessionControlWidget::PreviousButton:
			index = index.sibling( qMax( 0, index.row() -1 ), index.column() );
			break;
		case SessionControlWidget::NextButton:
			index = index.sibling( qMin( model()->rowCount() -1, index.row() +1 ), index.column() );
			break;
		case SessionControlWidget::FirstButton:
			index = index.sibling( 0, index.column() );
			break;
		case SessionControlWidget::LastButton:
			index = index.sibling( model()->rowCount() -1, index.column() );
			break;
	}
	
	setCurrentIndex( index );
	scrollTo( index, QAbstractItemView::PositionAtTop );
}
