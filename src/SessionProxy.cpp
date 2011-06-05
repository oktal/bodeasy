#include "SessionProxy.h"

#include <QVBoxLayout>

SessionProxy::SessionProxy( QWidget* parent )
    : QWidget( parent ),
      mRunning( false )
{
    QVBoxLayout* vl = new QVBoxLayout( this );
    vl->setMargin( 0 );
    vl->setSpacing( 0 );
}

QWidget* SessionProxy::widget() const
{
    return mWidget;
}

void SessionProxy::setWidget( QWidget* widget )
{
    if ( mWidget ) {
        if ( mWidget == widget ) {
            return;
        }
        
        delete mWidget;
    }
    
    mWidget = widget;
    
    if ( mWidget ) {
        layout()->addWidget( mWidget );
        connect( widget, SIGNAL( sessionFinished() ), this, SIGNAL( sessionFinished() ) );
    }
}

bool SessionProxy::isRunning() const
{
    return mRunning;
}

void SessionProxy::setRunning( bool running )
{
    mRunning = running;
}

void SessionProxy::setSessionId( qint64 id )
{
    QMetaObject::invokeMethod( mWidget, "setSessionId", Q_ARG( qint64, id ) );
}

void SessionProxy::setUserId( qint64 id )
{
    QMetaObject::invokeMethod( mWidget, "setUserId", Q_ARG( qint64, id ) );
}

void SessionProxy::refresh()
{
    QMetaObject::invokeMethod( mWidget, "refresh" );
}

void SessionProxy::start()
{
    QMetaObject::invokeMethod( mWidget, "start" );
}

void SessionProxy::stop()
{
    QMetaObject::invokeMethod( mWidget, "stop" );
}

void SessionProxy::showResults( qint64 sessionMadeId )
{
    QMetaObject::invokeMethod( mWidget, "showResults", Q_ARG( qint64, sessionMadeId ) );
}
