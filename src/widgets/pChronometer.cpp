#include "pChronometer.h"

#include <QTimer>
#include <QBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QPainter>
#include <QDebug>

pChronometer::pChronometer( QWidget* parent )
	: QFrame( parent )
{
	tTimer = new QTimer( this );
	tTimer->setInterval( 50 );
	mElapsed = 0;
	mFormat = "mm:ss:zzz";
	mPaused = false;
	mFont = QFont( "Monospace", 13, QFont::Bold );
	
	QMenu* menu = new QMenu( this );
	aStart = menu->addAction( tr( "Start" ) );
	aStart->setShortcut( QKeySequence( Qt::ControlModifier | Qt::Key_A ) );
	aStop = menu->addAction( tr( "Stop" ) );
	aStop->setShortcut( QKeySequence( Qt::ControlModifier | Qt::Key_O ) );
	aReset = menu->addAction( tr( "Reset" ) );
	aReset->setShortcut( QKeySequence( Qt::ControlModifier | Qt::Key_R ) );
	aPause = menu->addAction( tr( "Pause" ) );
	aPause->setShortcut( QKeySequence( Qt::ControlModifier | Qt::Key_P ) );
	aPause->setCheckable( true );
	
	QToolButton* tb = new QToolButton( this );
	tb->setIcon( QIcon::fromTheme( "chronometer" ) );
	tb->setPopupMode( QToolButton::InstantPopup );
	tb->setMenu( menu );
	
	QBoxLayout* ml = new QBoxLayout( QBoxLayout::LeftToRight, this );
	ml->addStretch();
	ml->addWidget( tb );
	
	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	
	connect( tTimer, SIGNAL( timeout() ), this, SLOT( timer_timeout() ) );
	connect( aStart, SIGNAL( triggered() ), this, SLOT( restart() ) );
	connect( aStop, SIGNAL( triggered() ), this, SLOT( stop() ) );
	connect( aReset, SIGNAL( triggered() ), this, SLOT( reset() ) );
	connect( aPause, SIGNAL( toggled( bool ) ), this, SLOT( setPaused( bool ) ) );
}

pChronometer::~pChronometer()
{
}

void pChronometer::restart()
{
	stop();
	mTime.start();
	tTimer->start();
}

void pChronometer::stop()
{
	tTimer->stop();
}

void pChronometer::reset()
{
	mTime.start();
	mElapsed = 0;
	update();
}

void pChronometer::setPaused( bool pause )
{
	if ( mPaused == pause ) {
		return;
	}
	
	if ( pause ) {
		mElapsed = mTime.elapsed();
		tTimer->stop();
	}
	else {
		mTime = QTime::currentTime().addMSecs( -mElapsed );
		tTimer->start();
	}
	
	mPaused = pause;
}

void pChronometer::timer_timeout()
{
	mElapsed = mTime.elapsed();
	update();
}

void pChronometer::paintEvent( QPaintEvent* event )
{
	QFrame::paintEvent( event );
	
	const QString text = QTime( 0, 0, 0 ).addMSecs( mElapsed ).toString( mFormat );
	const int flags = Qt::AlignCenter;
	QPainter painter( this );
	
	//qWarning() << "***" << mTime << text;
	
	painter.setFont( mFont );
	painter.setPen( Qt::red );
	painter.setBrush( Qt::NoBrush );
	painter.drawText( contentsRect(), flags, text );
}

