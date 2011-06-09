#ifndef SESSIONPROXY_H
#define SESSIONPROXY_H

#include <QWidget>
#include <QPointer>

#include "exercisewidgetdata.h"

class SessionProxy : public QWidget
{
	Q_OBJECT

public:
	enum Type {
		Unknown,
		Session,
		SessionMade
	};
	
	SessionProxy( QWidget* parent = 0 );
	
	QWidget* widget() const;
	void setWidget( QWidget* widget );

    bool isRunning() const;
	bool isReadOnly() const;

public slots:
	void setRunning( bool running, SessionProxy::Type type = SessionProxy::Unknown, bool readOnly = false );
    void setUserId( qint64 id );
	void setSessionId( qint64 id );
	void setSessionMadeId( qint64 id );
	void rollback();
	void updateModel();
	
    bool commit( const ExerciseWidgetDataList& data, bool askUser );
	void finishSession();

protected:
	QPointer<QWidget> mWidget;
	SessionProxy::Type mType;
	bool mRunning;
	bool mReadOnly;
	qint64 mUserId;
	qint64 mSessionId;
	qint64 mSessionMadeId;
	
	ExerciseWidgetDataList selectExercises() const;
	bool isCompleteExercise( const ExerciseWidgetData& data ) const;
	bool saveExerciseSeries( qint64 resultId );
	bool saveExercise( const ExerciseWidgetData& data );

signals:
	void error( const QString& error );
	void sessionStarted( bool readOnly );
	void sessionCommited( const ExerciseWidgetDataList& data );
	void sessionFinished();
};

#endif // SESSIONPROXY_H
