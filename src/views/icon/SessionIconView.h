#ifndef SESSIONICONVIEW_H
#define SESSIONICONVIEW_H

#include <QListView>

#include "exercisewidgetdata.h"

class SessionProxy;
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
	
	SessionProxy* sessionProxy() const;

public slots:
	void sessionUpdated( const ExerciseWidgetDataList& data, bool readOnly );
	void commitSession( bool askUser = false );

protected:
	SessionIconModel* mModel;
	SessionIconDelegate* mDelegate;

signals:
	void commitSession( const ExerciseWidgetDataList& data );
	void finishSession();
};

#endif // SESSIONICONVIEW_H
