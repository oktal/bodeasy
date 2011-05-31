#ifndef SESSIONICONVIEW_H
#define SESSIONICONVIEW_H

#include <QListView>

#include "SessionProxy.h"

class SessionIconModel;
class SessionIconDelegate;
class ExerciseWidget;

class SessionIconView : public QListView, public SessionProxy
{
	Q_OBJECT
	friend class SessionIconDelegate;
	
public:
	SessionIconView( QWidget* parent = 0 );
	virtual ~SessionIconView();

public slots:
	virtual void setSessionId( qint64 id );
    virtual void setUserId( qint64 id );
    virtual void refresh();
    virtual void start();
	virtual void stop();

protected:
	qint64 mSessionId;
    qint64 mUserId;
	SessionIconModel* mModel;
	SessionIconDelegate* mDelegate;
	QHash<qint64, ExerciseWidget*> mExercises;

protected slots:
	void selectExercises();
	//void on_btnFinish_clicked();

signals:
	virtual void sessionFinished();
};

#endif // SESSIONICONVIEW_H
