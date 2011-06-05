#ifndef SESSIONICONVIEW_H
#define SESSIONICONVIEW_H

#include <QListView>

class SessionIconModel;
class SessionIconDelegate;

class SessionIconView : public QListView
{
	Q_OBJECT
	friend class SessionIconDelegate;
	
public:
	SessionIconView( QWidget* parent = 0 );
	virtual ~SessionIconView();
	
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

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

protected slots:
	void selectExercises();
	//void on_btnFinish_clicked();

signals:
	virtual void sessionFinished();
};

#endif // SESSIONICONVIEW_H
