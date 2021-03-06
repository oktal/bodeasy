#ifndef PCHRONOMETER_H
#define PCHRONOMETER_H

#include <QFrame>
#include <QTime>

class QTimer;

class pChronometer : public QFrame
{
	Q_OBJECT
	
public:
	pChronometer( QWidget* parent = 0 );
	virtual ~pChronometer();
	
	virtual QSize sizeHint() const;
	
	QString textFormat() const;
	void setTextFormat( const QString& format );
	
	QColor textColor() const;
	void setTextColor( const QColor& color );
	
	QFont textFont() const;
	void setTextFont( const QFont& font );
	
	QString text() const;
	QRect textRect() const;

public slots:
	void restart();
	void stop();
	void reset();
	void setPaused( bool pause );

protected:
	QTimer* tTimer;
	QTime mTime;
	int mElapsed;
	QString mFormat;
	bool mPaused;
	QFont mFont;
	QColor mColor;
	
	QAction* aStart;
	QAction* aStop;
	QAction* aReset;
	QAction* aPause;

protected slots:
	void timer_timeout();
	
	virtual void paintEvent( QPaintEvent* event );
};

#endif // PCHRONOMETER_H
