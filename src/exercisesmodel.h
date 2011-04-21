#ifndef EXERCISESMODEL_H
#define EXERCISESMODEL_H

#include <QAbstractListModel>

#include "exercise.h"

class ExercisesManager;

class ExercisesModel : public QAbstractListModel
{
	Q_OBJECT
	
public:
	explicit ExercisesModel( QObject* parent = 0 );
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    void removeRow( int row, const QModelIndex& parent = QModelIndex() );
	bool addExercise( Exercise& exercise );
	QModelIndex index( const Exercise& exercise ) const;

protected:
	ExercisesManager* mManager;
    QList<Exercise> mExercises;
};

#endif // EXERCISESMODEL_H
