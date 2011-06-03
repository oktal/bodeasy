#ifndef SESSIONPROXY_H
#define SESSIONPROXY_H

#include <QWidget>
#include <QPointer>

class SessionProxy : public QWidget
{
	Q_OBJECT

public:

	SessionProxy( QWidget* parent = 0 );
	
	QWidget* widget() const;
	void setWidget( QWidget* widget );

public slots:
	virtual void setSessionId( qint64 id );
    virtual void setUserId( qint64 id );
    virtual void refresh();
    virtual void start();
	virtual void stop();
    virtual void showResults( qint64 sessionMadeId );

protected:
	QPointer<QWidget> mWidget;

signals:
	void sessionFinished();
};

#endif // SESSIONPROXY_H
