#include "SessionIconDelegate.h"
#include "SessionIconModel.h"
#include "SessionIconView.h"
#include "exercisewidget.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QTime>

SessionIconDelegate::SessionIconDelegate( QObject* parent )
	: QStyledItemDelegate( parent )
{
}

SessionIconDelegate::~SessionIconDelegate()
{
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
			drawExercise( painter, option, index );
			return;
		}
	}
	
	QStyledItemDelegate::paint( painter, option, index );
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
		model->setData( index, QVariant::fromValue( ed->data() ), Qt::EditRole );
		return;
	}
	
	QStyledItemDelegate::setModelData( editor, model, index );
}

QSize SessionIconDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return index.isValid() ? QSize( 450, 300 ) : QStyledItemDelegate::sizeHint( option, index );
}

QString SessionIconDelegate::exerciseWidgetTr( const QString& string ) const
{
	return QApplication::translate( "ExerciseWidget", string.toLatin1().constData() );
}

QString SessionIconDelegate::typeToString( Exercise::Type type ) const
{
	switch ( type ) {
		case Exercise::Repetition:
			return exerciseWidgetTr( QString::fromUtf8( "Répétition" ) );
		case Exercise::Duration:
			return exerciseWidgetTr( QString::fromUtf8( "Durée" ) );
    }
	
	return QString::null;
}

QString SessionIconDelegate::difficultyToString( Exercise::Difficulty difficulty ) const
{
	switch ( difficulty ) {
		case Exercise::Easy:
			return exerciseWidgetTr( QString::fromUtf8( "Facile" ) );
		case Exercise::Medium:
			return exerciseWidgetTr( QString::fromUtf8( "Moyen" ) );
		case Exercise::Hard:
			return exerciseWidgetTr( QString::fromUtf8( "Difficile" ) );
    }
	
	return QString::null;
}

void SessionIconDelegate::drawFakeLabel( QPainter* painter, const QStyleOptionFrameV3& option, const QString& text, Qt::Alignment align, Qt::TextFormat format, QStyle* style, QWidget* widget ) const
{
	if ( !style ) {
		style = QApplication::style();
	}
	
	// fucking hack for background painting ???
	painter->setPen( Qt::NoPen );
	painter->setBrush( option.palette.color( QPalette::Window ) );
	painter->drawRect( option.rect );
	
	style->drawControl( QStyle::CE_ShapedFrame, &option, painter, widget );
	
	const QRect rect = option.rect.adjusted( option.lineWidth, option.lineWidth, -option.lineWidth, -option.lineWidth );
	QTextOption textOption;
	
	textOption.setAlignment( align );
	textOption.setTabStop( 40 );
	textOption.setAlignment( align );
	textOption.setWrapMode( QTextOption::WrapAtWordBoundaryOrAnywhere );
	
	mDocument.setDefaultFont( painter->font() );
	mDocument.setDocumentMargin( 0 );
	mDocument.setDefaultTextOption( textOption );
	
	switch ( format ) {
		case Qt::AutoText:
			if ( Qt::mightBeRichText( text ) ) {
				format = Qt::RichText;
			}
			else {
				format = Qt::PlainText;
			}
		case Qt::RichText:
			mDocument.setHtml( text );
			break;
		case Qt::PlainText:
		case Qt::LogText:
			mDocument.setPlainText( text );
			break;
	}
	
	if ( mDocument.size().width() > rect.width() ) {
		mDocument.setTextWidth( rect.width() );
	}
	
	QRect r = QRect( rect.topLeft(), mDocument.size().toSize() );
	
	// horizontal
	if ( align & Qt::AlignLeft ) {
		// nothing to do
	}
	else if ( align & Qt::AlignRight ) {
		r.moveRight( rect.right() );
	}
	else if ( align & Qt::AlignHCenter ) {
		r.moveCenter( QPoint( rect.center().x(), r.center().y() ) );
	}
	else if ( align & Qt::AlignJustify ) {
		r.moveCenter( QPoint( rect.center().x(), r.center().y() ) );
	}
	
	// vertical
	if ( align & Qt::AlignTop ) {
		r.moveTop( rect.top() );
	}
	else if ( align & Qt::AlignBottom ) {
		if ( r.height() <= rect.height() ) {
			r.moveBottom( rect.bottom() );
		}
	}
	else if ( align & Qt::AlignVCenter ) {
		if ( r.height() <= rect.height() ) {
			r.moveCenter( QPoint( r.center().x(), rect.center().y() ) );
		}
	}
	
	// absolute
	if ( align & Qt::AlignAbsolute ) {
		// nothing to do
	}
	
	painter->translate( r.topLeft() );
	mDocument.drawContents( painter, QRect( QPoint(), QSize( r.size().width(), rect.height() ) ) );
	painter->translate( -r.topLeft() );
}

QStyleOptionFrameV3 makeStyleOptionFrame( const QStyleOptionViewItem& option, int frameStyle, int lineWidth = 1, int midLineWidth = 1, QStyleOptionFrameV3::FrameFeatures features = QStyleOptionFrameV2::None )
{
	const int frameShape  = frameStyle & QFrame::Shape_Mask;
    const int frameShadow = frameStyle & QFrame::Shadow_Mask;
	QStyleOptionFrameV3 opt;
	
	opt.QStyleOption::operator=( option );
	opt.frameShape = QFrame::Shape( int( opt.frameShape ) | frameShape );
	opt.features = features;
	
	switch ( frameShape ) {
		case QFrame::Box:
		case QFrame::HLine:
		case QFrame::VLine:
		case QFrame::StyledPanel:
		case QFrame::Panel:
			opt.lineWidth = lineWidth;
			opt.midLineWidth = midLineWidth;
			break;
		default:
			// most frame styles do not handle customized line and midline widths
			// (see updateFrameWidth()).
			//opt.lineWidth = d->frameWidth;
			opt.lineWidth = lineWidth;
			break;
	}
	
	if ( frameShadow == QFrame::Sunken ) {
		opt.state |= QStyle::State_Sunken;
	}
	else if ( frameShadow == QFrame::Raised ) {
		opt.state |= QStyle::State_Raised;
	}
	
	return opt;
}

void SessionIconDelegate::drawExercise( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	const ExerciseWidgetData data = index.data( SessionIconModel::ExerciseDataRole ).value<ExerciseWidgetData>();
	const int margin = 5;
	const int spacing = 5;
	const int headerHeight = 29;
	const int descriptionTitleHeight = 19;
	const QRect rect = option.rect.adjusted( margin, margin, -margin, -margin );
	const int boxWidth = ( ( rect.width() /2 ) -( spacing *2 ) ) /2;
	const int boxHeight = ( ( rect.height() /2 ) -headerHeight -( spacing *4 ) ) /3;
	
	painter->setRenderHint( QPainter::Antialiasing, true );
	
	// fill background
	painter->setPen( Qt::NoPen );
	painter->setBrush( option.palette.brush( QPalette::Window ) );
	painter->drawRoundedRect( option.rect, margin, margin );
	
	// draw selection / hover
	if ( option.state & QStyle::State_Enabled && ( option.state & QStyle::State_Selected || option.state & QStyle::State_MouseOver ) ) {
		const QColor highlight = option.palette.brush( QPalette::Highlight ).color();
		const QColor lighter = highlight.lighter( 150 );
		const QColor darker = highlight.darker( 140 );
		const int radius = 5;
		QLinearGradient gradient( option.rect.topLeft(), option.rect.bottomLeft() );
		gradient.setColorAt( 0, lighter );
		gradient.setColorAt( 1, option.state & QStyle::State_Selected ? highlight : darker );
		
		painter->setPen( QPen( darker, 1 ) );
        painter->setBrush( gradient );
        painter->drawRoundedRect( option.rect.adjusted( 1, 1, darker != Qt::NoPen ? -1 : 0, darker != Qt::NoPen ? -1 : 0 ), radius, radius );
	}
	
	// header
	QStyleOptionFrameV3 headerOption = makeStyleOptionFrame( option, QFrame::StyledPanel | QFrame::Sunken );
	headerOption.rect = QRect( rect.topLeft(), QSize( rect.width(), headerHeight ) );
	headerOption.palette.setColor( QPalette::Window, QColor( 232, 232, 232 ) );
	headerOption.palette.setColor( QPalette::Dark, QColor( 117, 117, 117 ) );
	drawFakeLabel( painter, headerOption, QString( "<b>%1</b>" ).arg( data.name ) );
	
	// draw type, difficulty, repetition, rest
	const QPoint boxOrigin = headerOption.rect.bottomLeft() +QPoint( ( rect.width() /2 ) +spacing, spacing );
	QStyleOptionFrameV3 boxOption = makeStyleOptionFrame( option, QFrame::NoFrame );
	boxOption.rect = QRect( rect.topLeft(), QSize( boxWidth, boxHeight ) );
	boxOption.palette.setColor( QPalette::Window, QColor( 232, 232, 232 ) );
	boxOption.palette.setColor( QPalette::WindowText, QColor( 0, 0, 0 ) );
	
	for ( int x = 0; x < 2; x++ ) {
		for ( int y = 0; y < 4; y++ ) {
			const int xx = ( x *spacing ) +( x *boxWidth );
			const int yy = ( y *spacing ) +( y *boxHeight );
			bool bold = false;
			QString text;
			
			boxOption.rect.moveTopLeft( boxOrigin +QPoint( xx, yy ) );
			
			switch ( y ) {
				case 0: {
					if ( x == 0 ) {
						bold = true;
						text = exerciseWidgetTr( QString::fromUtf8( "Type" ) );
					}
					else {
						text = typeToString( data.type );
					}
					
					break;
				}
				case 1: {
					if ( x == 0 ) {
						bold = true;
						text = exerciseWidgetTr( QString::fromUtf8( "Difficulté" ) );
					}
					else {
						text = difficultyToString( data.difficulty );
					}
					
					break;
				}
				case 2: {
					if ( x == 0 ) {
						bold = true;
						text = exerciseWidgetTr( QString::fromUtf8( "Répétitions" ) );
					}
					else {
						text = QString::number( data.repetitions );
					}
					
					break;
				}
				case 3: {
					if ( x == 0 ) {
						bold = true;
						text = exerciseWidgetTr( QString::fromUtf8( "Repos" ) );
					}
					else {
						text = exerciseWidgetTr( QString::fromUtf8( "%1 secs." ) ).arg( data.rest );
					}
					
					break;
				}
			}
			
			if ( bold ) {
				text = QString( "<b>%1</b>" ).arg( text );
			}
			
			drawFakeLabel( painter, boxOption, text );
		}
	}
	
	// draw description title
	QStyleOptionFrameV3 descriptionTitleOption = makeStyleOptionFrame( option, QFrame::NoFrame );
	descriptionTitleOption.rect = QRect( boxOption.rect.topLeft() +QPoint( -( spacing +boxWidth ), boxHeight +spacing ), QSize( ( rect.width() /2 ) -spacing, descriptionTitleHeight ) );
	descriptionTitleOption.palette.setColor( QPalette::Window, QColor( 0, 0, 0, 0 ) );
	drawFakeLabel( painter, descriptionTitleOption, exerciseWidgetTr( QString::fromUtf8( "Description" ) ), Qt::AlignLeft | Qt::AlignVCenter );
	
	// draw description
	QStyleOptionFrameV3 descriptionOption = makeStyleOptionFrame( option, QFrame::StyledPanel | QFrame::Sunken, 1, 0 );
	descriptionOption.rect = descriptionTitleOption.rect.adjusted( 0, descriptionTitleOption.rect.height() +spacing, 0, ( ( rect.height() -headerHeight -spacing ) /2 ) -spacing -descriptionTitleHeight -spacing -spacing -spacing );
	descriptionOption.palette.setColor( QPalette::Window, QColor( 255, 255, 255 ) );
	drawFakeLabel( painter, descriptionOption, data.description, Qt::AlignLeft | Qt::AlignTop );
}
