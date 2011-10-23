#ifndef SESSIONICONVIEW_H
#define SESSIONICONVIEW_H

#include <QListView>

#include "widgets/sessioncontrolwidget.h"
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
	Q_INVOKABLE bool objectiveDone() const;
    Q_INVOKABLE QString comment() const;
    Q_INVOKABLE void setWidgetsData( const ExerciseWidgetDataList& data, const QString& objective, bool objectiveDone, const QString& comment, bool readOnly );
	
protected:
	SessionProxy* mProxy;
	SessionIconModel* mModel;
	SessionIconDelegate* mDelegate;
	QFrame* mSeparator;
	SessionControlWidget* mControl;
	
	virtual bool eventFilter( QObject* o, QEvent* e );
	virtual void selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );

protected slots:
	void buttonClicked( SessionControlWidget::Button button );
};

#endif // SESSIONICONVIEW_H
