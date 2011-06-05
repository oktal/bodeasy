#include "SessionIconDelegate.h"
#include "SessionIconModel.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QPixmapCache>
#include <QDebug>
#include <QTime>
#include <QApplication>

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
		QPalette pal = QApplication::palette();
		pal.setColor( editor->backgroundRole(), pal.color( QPalette::Window ) );
		editor->setPalette( pal );
		return editor;
	}
	
	return QStyledItemDelegate::createEditor( parent, option, index );
}

void SessionIconDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( index.isValid() ) {
		if ( !( option.state & QStyle::State_Editing ) ) {
			painter->save();
			drawExercise( painter, option, index );
			painter->restore();
		}
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

QString SessionIconDelegate::typeToString( Exercise::Type type ) const
{
	switch ( type ) {
		case Exercise::Repetition:
			return trUtf8( "Répétition" );
		case Exercise::Duration:
			return trUtf8( "Durée" );
    }
	
	return QString::null;
}

QString SessionIconDelegate::difficultyToString( Exercise::Difficulty difficulty ) const
{
	switch ( difficulty ) {
		case Exercise::Easy:
			return trUtf8( "Facile" );
		case Exercise::Medium:
			return trUtf8( "Moyen" );
		case Exercise::Hard:
			return trUtf8( "Difficile" );
    }
	
	return QString::null;
}

void SessionIconDelegate::drawFrame( QPainter* painter, const QRect& rect, int radius, const QBrush& pen, const QBrush& brush, const QString& text, const QBrush& color, int flags, const QFont& font, int padding ) const
{
	painter->setPen( QPen( pen, 1 ) );
	painter->setBrush( brush );
	painter->drawRoundedRect( rect.adjusted( 0, 0, pen != Qt::NoBrush ? -1 : 0, pen != Qt::NoBrush ? -1 : 0 ), radius, radius );
	
	if ( !text.isEmpty() ) {
		painter->setFont( font );
		painter->setPen( QPen( color, 1 ) );
		painter->setBrush( Qt::NoBrush );
		painter->drawText( rect.adjusted( radius +( flags & Qt::AlignHCenter ? 0: padding ), radius +( flags & Qt::AlignVCenter ? 0: padding ), -radius, -radius ), flags, text );
	}
}

void SessionIconDelegate::drawExercise( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
	const bool isEnabled = option.state & QStyle::State_Enabled;
	const bool isSelected = option.state & QStyle::State_Selected;
	const bool isHovered = option.state & QStyle::State_MouseOver;
	const int margin = 5;
	const int boxSpacing = 5;
	const QRect rect = option.rect.adjusted( margin, margin, -margin, -margin );
	const QRect nameRect = rect.adjusted( 0, 0, 0, -( rect.height() /100.0 *90.0 ) );
	const QRect rightRect = rect.adjusted( ( rect.width() /2 ) +boxSpacing, nameRect.bottom() +boxSpacing, 0, 0 );
	QRect descriptionRect = rightRect.adjusted( 0, ( rightRect.height() /2 ) +boxSpacing, 0, 0 );
	const QRect descriptionTitleRect = descriptionRect.adjusted( 0, boxSpacing, 0, -( descriptionRect.height() /100.0 *80.0 ) );
	descriptionRect = descriptionRect.adjusted( 0, descriptionRect.height() /100.0 *25.0, 0, 0 );
	const QFont font = painter->font();
	QFont nameFont = painter->font();
	
	nameFont.setBold( true );
	
	painter->setRenderHint( QPainter::Antialiasing );
	
	// fill background
	painter->setPen( Qt::NoPen );
	painter->setBrush( option.palette.brush( QPalette::Window ) );
	painter->drawRoundedRect( option.rect, 5, 5 );
	
	// draw selection / hover
	if ( isEnabled && ( isSelected || isHovered ) ) {
		const QColor highlight = option.palette.brush( QPalette::Highlight ).color();
		const QColor lighter = highlight.lighter( 150 );
		const QColor darker = highlight.darker( 140 );
		QLinearGradient gradient( option.rect.topLeft(), option.rect.bottomLeft() );
		gradient.setColorAt( 0, lighter );
		gradient.setColorAt( 1, isSelected ? highlight : darker );
		
		drawFrame( painter, option.rect, 5, darker, gradient );
	}
	
	// draw exercise name
	drawFrame( painter, nameRect, 0, QColor( 117, 117, 117 ), QColor( 232, 232, 232 ), data.name, QColor( Qt::black ), Qt::AlignVCenter | Qt::AlignLeft, nameFont );
	
	// draw type, difficulty, repetition, rest
	for ( int x = 0; x < 2; x++ ) {
		const int width = ( rightRect.width() -( boxSpacing *1 ) ) /2;
		const int height = ( rightRect.height() -( boxSpacing *3 ) ) /8;
		
		for ( int y = 0; y < 4; y++ ) {
			const QPoint sp = QPoint( boxSpacing *x, boxSpacing *y );
			const QPoint p = QPoint( width *x, height *y );
			QRect r = QRect( QPoint( rightRect.topLeft() ) +sp +p, QSize( width, height ) );
			QFont f = font;
			QString t;
			
			switch ( y ) {
				case 0: {
					if ( x == 0 ) {
						f.setBold( true );
						t = trUtf8( "Type" );
					}
					else {
						t = typeToString( data.type );
					}
					
					break;
				}
				case 1: {
					if ( x == 0 ) {
						f.setBold( true );
						t = trUtf8( "Difficulté" );
					}
					else {
						t = difficultyToString( data.difficulty );
					}
					
					break;
				}
				case 2: {
					if ( x == 0 ) {
						f.setBold( true );
						t = trUtf8( "Répétitions" );
					}
					else {
						t = QString::number( data.repetitions );
					}
					
					break;
				}
				case 3: {
					if ( x == 0 ) {
						f.setBold( true );
						t = trUtf8( "Repos" );
					}
					else {
						t = tr( "%1 secs." ).arg( data.rest );
					}
					
					break;
				}
			}
			
			drawFrame( painter, r, 0, QColor( 117, 117, 117 ), QColor( 232, 232, 232 ), t, QColor( Qt::black ), Qt::AlignCenter, f );
		}
	}
	
	// draw description
	drawFrame( painter, descriptionTitleRect, 0, Qt::NoBrush, Qt::NoBrush, tr( "Description" ), QColor( Qt::black ), Qt::AlignVCenter | Qt::AlignLeft, font );
	drawFrame( painter, descriptionRect, 0, Qt::black, Qt::white, tr( "test de merde \nsur plusieurs ligne de bruns qui put\n hihihih\test :)" ), Qt::black, Qt::AlignLeft | Qt::AlignTop, font );
}
