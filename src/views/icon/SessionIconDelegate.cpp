#include "SessionIconDelegate.h"
#include "SessionIconModel.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QPixmapCache>
#include <QDebug>
#include <QTime>

SessionIconDelegate::SessionIconDelegate( QObject* parent )
	: QStyledItemDelegate( parent ),
	mWidget( new ExerciseWidget )
{
	mWidget->resize( QSize( 450, 300 ) );
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
		editor->setAutoFillBackground( true );
		return editor;
	}
	
	return QStyledItemDelegate::createEditor( parent, option, index );
}

void SessionIconDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.isValid() ) {
		const bool isEditing = option.state & QStyle::State_Editing;
		
		//if ( !isEditing ) {
			//qWarning() << "Painting pixmap" << QTime::currentTime();
			//painter->drawPixmap( option.rect.topLeft(), cachedPixmap( index ) );
			
			const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
			mWidget->setData( data );
			mWidget->render( painter, option.rect.topLeft() +QPoint( 276, 43 ), QRegion(), QWidget::DrawWindowBackground | QWidget::DrawChildren );
			//painter->drawRect( option.rect.adjusted( 0, 0, -1, -1 ) );
		//}
		//else {
			//qWarning() << "NOT Painting pixmap" << QTime::currentTime();
		//}
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
		if ( model->setData( index, QVariant::fromValue( ed->data() ), Qt::EditRole ) ) {
			/*mWidget->setData( ed->data() );
			cachePixmap( index, QPixmap::grabWidget( mWidget ) );*/
		}
	}
	else {
		QStyledItemDelegate::setModelData( editor, model, index );
	}
}

QSize SessionIconDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return index.isValid() ? mWidget->size() : QStyledItemDelegate::sizeHint( option, index );
}

/*QString SessionIconDelegate::indexKey( const QModelIndex& index ) const
{
	return QString( "%1-%2" ).arg( index.row() ).arg( index.data( Qt::DisplayRole ).toString() );
}
#include <QLabel>
void SessionIconDelegate::cachePixmap( const QModelIndex& index, const QPixmap& pixmap ) const
{
	const QString key = indexKey( index );
	static QLabel* l1 = 0;
	static QLabel* l2 = 0;
	
	if ( !l1 ) {
		l1 = new QLabel;
		l1->setWindowTitle( "l1" );
		l2 = new QLabel;
		l2->setWindowTitle( "l2" );
	}
	
	if ( !QPixmapCache::insert( key, pixmap ) ) {
		qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << key << index;
	}
	
	l1->setPixmap( pixmap );
	l2->setPixmap( cachedPixmap( index ) );
	
	l1->show();
	l2->show();
}

QPixmap SessionIconDelegate::cachedPixmap( const QModelIndex& index ) const
{
	const QString key = indexKey( index );
	QPixmap pixmap;
	
	if ( !QPixmapCache::find( key, pixmap ) ) {
		const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
		mWidget->setData( data );
		pixmap = QPixmap::grabWidget( mWidget );
		cachePixmap( index, pixmap );
	}
	
	return pixmap;
}*/
