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
	SessionIconView( SessionProxy* proxy );
	virtual ~SessionIconView();
	
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	
	Q_INVOKABLE ExerciseWidgetDataList widgetsData() const;
	Q_INVOKABLE void setWidgetsData( const ExerciseWidgetDataList& data, bool readOnly );


	
protected:
	SessionProxy* mProxy;
	SessionIconModel* mModel;
	SessionIconDelegate* mDelegate;
};

#endif // SESSIONICONVIEW_H
