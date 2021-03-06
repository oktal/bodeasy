#include "exercisesmodel.h"
#include "sql/managers/exercisesmanager.h"

#include <QDebug>

ExercisesModel::ExercisesModel( QObject* parent )
	: QAbstractListModel( parent )
{
    mManager = new ExercisesManager( this );
    mExercises = mManager->selectExercises();
}

QVariant ExercisesModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= mExercises.count() || index.column() != 0 ) {
        return QVariant();
    }
	
    switch ( role ) {
        case Qt::DisplayRole:
            return mExercises[ index.row() ].name;
            break;
        case Qt::UserRole:
            return mExercises[ index.row() ].id;
            break;
    }
	
    return QVariant();
}

int ExercisesModel::rowCount( const QModelIndex& parent ) const
{
    return parent.isValid() ? 0 : mExercises.count();
}

void ExercisesModel::removeRow( int row, const QModelIndex& parent )
{
    if ( parent.isValid() || row < 0 || row >= mExercises.count() ) {
        return;
    }
	
    beginRemoveRows( parent, row, row );
    if ( mManager->removeExerciseById( mExercises[ row ].id ) ) {
        mExercises.removeAt( row );
    }
    endRemoveRows();
}

bool ExercisesModel::addExercise( Exercise& exercise )
{
    if ( mManager->createExercise( exercise ) ) {
        beginInsertRows( QModelIndex(), mExercises.count(), mExercises.count() );
        mExercises.append( exercise );
        endInsertRows();
        return true;
    }
	
    return false;
}

bool ExercisesModel::updateExercise( const Exercise &exercise )
{
    if ( mManager->updateExercise( exercise ) )
    {
        mExercises = mManager->selectExercises();
        emit dataChanged( QModelIndex(), QModelIndex() );
        return true;
    }

    return false;
}

QModelIndex ExercisesModel::index( const Exercise& exercise ) const
{
    for ( int i = 0; i < mExercises.count(); i++ ) {
        const Exercise& e = mExercises[ i ];

        if ( e.name.compare( exercise.name, Qt::CaseInsensitive ) == 0 ) {
            return QAbstractListModel::index( i );
        }
     }
	
    return QModelIndex();
}

Exercise ExercisesModel::exercise( const QModelIndex &index ) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() > mExercises.count() )
    {
        return Exercise();
    }

    return mExercises[ index.row() ];
}
