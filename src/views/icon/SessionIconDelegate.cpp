#include "SessionIconDelegate.h"
#include "SessionIconModel.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QApplication>
#include <QPixmapCache>
#include <QDebug>

SessionIconDelegate::SessionIconDelegate( QObject* parent )
	: QStyledItemDelegate( parent ), mEditor( new ExerciseWidget )
{
    mEditor->setReadOnly( true );
}

SessionIconDelegate::~SessionIconDelegate()
{
	delete mEditor;
}

QWidget* SessionIconDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.column() == 0 ) {
		ExerciseWidget* editor = new ExerciseWidget( parent );
		editor->setFocusPolicy( Qt::StrongFocus );
		editor->setAutoFillBackground( true );
        QPalette pal = QApplication::palette();
		pal.setColor( editor->backgroundRole(), pal.color( QPalette::Window ) );
		editor->setPalette( pal );
		return editor;
	}
	
	return QStyledItemDelegate::createEditor( parent, option, index );
}

void SessionIconDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( !index.isValid() || ( option.state & QStyle::State_Editing ) ) {
		return;
	}
	
	painter->drawPixmap( option.rect.topLeft(), cachedEditor( index, option ) );
}

void SessionIconDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
	const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
	ExerciseWidget* ed = qobject_cast<ExerciseWidget*>( editor );
	
	if ( ed ) {
		ed->setData( data );
		return;
	}
	
	QStyledItemDelegate::setEditorData( editor, index );
}

void SessionIconDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
	ExerciseWidget* ed = qobject_cast<ExerciseWidget*>( editor );
	
	if ( ed ) {
		if ( model->setData( index, QVariant::fromValue( ed->data() ), Qt::EditRole ) ) {
			invalidateCachedEditor( index );
		}
		
		return;
	}
	
	QStyledItemDelegate::setModelData( editor, model, index );
}

QSize SessionIconDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	Q_UNUSED( option );
	Q_UNUSED( index );
	return QSize( 450, 300 );
	//return index.isValid() ? QSize( 450, 300 ) : QStyledItemDelegate::sizeHint( option, index );
}

void SessionIconDelegate::clearCachedEditors( QAbstractItemModel* model ) const
{
	for ( int i = 0; i < model->rowCount(); i++ ) {
		const QModelIndex index = model->index( i, 0 );
		invalidateCachedEditor( index );
	}
}

QString SessionIconDelegate::cachedEditorKey( const QModelIndex& index, const QStyleOptionViewItem& option ) const
{
	if ( !index.isValid() ) {
		return QString::null;
	}
	
	const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
	QStringList parts;
	
	parts << QString::number( data.number );
	
	if ( option.state & QStyle::State_Selected ) {
		parts << "Selected";
	}
	else if ( option.state & QStyle::State_MouseOver ) {
		parts << "Hover";
	}
	
	parts << "SessionIconDelegate";
	
	
	return parts.join( ":" );
}

QPixmap SessionIconDelegate::cachedEditor( const QModelIndex& index, const QStyleOptionViewItem& option ) const
{
	const QString key = cachedEditorKey( index, option );
	QPixmap pixmap;
	
	if ( !QPixmapCache::find( key, pixmap ) ) {
		QPalette pal = QApplication::palette();
		
		if ( option.state & QStyle::State_Selected ) {
			pal.setColor( mEditor->backgroundRole(), pal.color( QPalette::Highlight ) );
		}
		else if ( option.state & QStyle::State_MouseOver ) {
			pal.setColor( mEditor->backgroundRole(), pal.color( QPalette::Highlight ).lighter() );
		}
		
		setEditorData( mEditor, index );
		mEditor->setPalette( pal );
		mEditor->resize( option.rect.size() );
		mEditor->ensurePolished();
		
		pixmap = QPixmap( mEditor->size() );
		pixmap.fill( QColor( Qt::transparent ) );
		
		QPainter painter( &pixmap );
		mEditor->render( &painter );
		
		if ( !QPixmapCache::insert( key, pixmap ) ) {
			qWarning() << Q_FUNC_INFO << "Can't cache editor" << index.data( Qt::DisplayRole ).toString();
		}
	}
	
	return pixmap;
}

void SessionIconDelegate::invalidateCachedEditor( const QModelIndex& index ) const
{
	QStringList parts = cachedEditorKey( index, QStyleOptionViewItem() ).split( ":" );
	QPixmapCache::remove( parts.join( ":" ) );
	parts.insert( 1, "Selected" );
	QPixmapCache::remove( parts.join( ":" ) );
	parts.replace( 1, "Hover" );
	QPixmapCache::remove( parts.join( ":" ) );
}

