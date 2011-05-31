#ifndef SESSIONPROXY_H
#define SESSIONPROXY_H

#include <QtGlobal>

class SessionProxy
{
public:
	// public slots
	virtual void setSessionId( qint64 id ) = 0;
    virtual void setUserId( qint64 id ) = 0;
    virtual void refresh() = 0;
    virtual void start() = 0;
	virtual void stop() = 0;

	// signals
	virtual void sessionFinished() = 0;
};

#endif // SESSIONPROXY_H
