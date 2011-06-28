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

    enum ViewMode {
        None,
        List,
        Page
    };
	
	SessionProxy( QWidget* parent = 0 );

    ViewMode viewMode() const;
    void setViewMode(ViewMode mode);

	qint64 userId() const;
	qint64 sessionId() const;
	qint64 sessionMadeId() const;
	
    bool isRunning() const;
	SessionProxy::Type type() const;
	bool isReadOnly() const;
	bool isModified() const;

public slots:
	bool setRunning( bool running, SessionProxy::Type type, bool readOnly, bool askConfirmation = true );
	bool stop( bool askConfirmation = true );
    void setUserId( qint64 id );
	void setSessionId( qint64 id );
	void setSessionMadeId( qint64 id );
	void updateModel();
	
    bool commit( const ExerciseWidgetDataList& data, bool objectiveAchieved, const QString &comment );
	void rollback();

protected:
	QPointer<QWidget> mWidget;
	SessionProxy::Type mType;
	bool mRunning;
	bool mReadOnly;
	qint64 mUserId;
	qint64 mSessionId;
	qint64 mSessionMadeId;
    ViewMode mViewMode;
	
	ExerciseWidgetDataList selectExercises() const;
    QString selectObjective() const;
	bool selectObjectiveDone() const;
    QString selectComment() const;
	bool isModified( const ExerciseWidgetDataList& data ) const;
	bool isCompleteExercise( const ExerciseWidgetData& data ) const;
	bool saveExerciseSeries( qint64 resultId );
	bool saveExercise( const ExerciseWidgetData& data );

    QWidget* widget() const;
    void setWidget( QWidget* widget );

signals:
	void error( const QString& error );
	void sessionStarted( SessionProxy::Type type, bool readOnly );
	void sessionCommited( const ExerciseWidgetDataList& data );
	void sessionFinished();
};

#endif // SESSIONPROXY_H
