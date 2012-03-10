#ifndef SESSIONICONMODEL_H
#define SESSIONICONMODEL_H

#include <QStringListModel>

#include "exercisewidgetdata.h"

class SessionIconModel : public QStringListModel
{
	Q_OBJECT
	
public:
	enum CustomRole {
		ExerciseDataRole = Qt::UserRole
	};
	
	SessionIconModel( QObject* parent = 0 );
	virtual ~SessionIconModel();
	
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	ExerciseWidgetDataList widgetsData() const;
	void setWidgetsData( const ExerciseWidgetDataList& widgetsData );
	
	bool isReadOnly() const;
	void setReadOnly( bool readOnly );

protected:
	ExerciseWidgetDataList mData;
	bool mReadOnly;
	
	void clear();
};

#endif // SESSIONICONMODEL_H
