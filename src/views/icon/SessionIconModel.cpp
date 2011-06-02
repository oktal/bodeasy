#include "SessionIconModel.h"

SessionIconModel::SessionIconModel( QObject* parent )
	: QStringListModel( parent )
{
}

SessionIconModel::~SessionIconModel()
{
}

QVariant SessionIconModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() ) {
		return QVariant();
	}
	
	switch ( role ) {
		case Qt::DisplayRole:
			return mData.value( index.row() ).name;
		case SessionIconModel::ExerciseDataRole:
			return QVariant::fromValue( mData.value( index.row() ) );
	}
	
	return QVariant();
}

int SessionIconModel::rowCount( const QModelIndex& parent ) const
{
	return parent == QModelIndex() ? mData.count() : 0;
}

bool SessionIconModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	if ( !index.isValid() ) {
		return false;
	}
	
	switch ( role ) {
		case Qt::EditRole:
			mData[ index.row() ] = value.value<ExerciseWidgetData>();
			break;
		default:
			return false;
	}
	
	emit dataChanged( index, index );
	return true;
}

ExerciseWidgetDataList SessionIconModel::widgetsData() const
{
	return mData;
}

void SessionIconModel::setWidgetsData( const ExerciseWidgetDataList& widgetsData )
{
	clear();
	
	if ( widgetsData.isEmpty() ) {
		return;
	}
	
	const int count = widgetsData.count();
	
	beginInsertRows( QModelIndex(), 0, count -1 );
	mData = widgetsData;
	endInsertRows();
}

void SessionIconModel::clear()
{
	if ( mData.isEmpty() ) {
		return;
	}
	
	const int count = mData.count();
	
	beginRemoveRows( QModelIndex(), 0, count -1 );
	mData.clear();
	endRemoveRows();
}
