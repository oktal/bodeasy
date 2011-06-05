#include "SessionIconView.h"
#include "SessionIconModel.h"
#include "SessionIconDelegate.h"
#include "exercisewidget.h"
#include "sql/SqlHelper.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

SessionIconView::SessionIconView( QWidget* parent )
	: QListView( parent ),
	mSessionId( -1 ),
	mUserId( -1 ),
	mModel( new SessionIconModel( this ) ),
	mDelegate( new SessionIconDelegate( this ) )
{
	setUniformItemSizes( true );
	setAlternatingRowColors( false );
	setResizeMode( QListView::Adjust );
	setViewMode( QListView::IconMode );
	setMovement( QListView::Static );
	setSpacing( 5 );
	setModel( mModel );
	setItemDelegate( mDelegate );
}

SessionIconView::~SessionIconView()
{
}

void SessionIconView::setSessionId( qint64 id )
{
	mSessionId = id;
    refresh();
}

void SessionIconView::setUserId( qint64 id )
{
	mUserId = id;
}

void SessionIconView::refresh()
{
	selectExercises();
}

void SessionIconView::start()
{
}

void SessionIconView::stop()
{
    /*QList<ExerciseWidget*>::const_iterator it;

    bool isComplete = true;

    // Check if each exercise is complete
	foreach ( ExerciseWidget* ew, mExercises ) {
        if ( !ew->isComplete() ) {
            isComplete = false;
            break;
        }
    }
	
    if ( !isComplete ) {
        const int r = QMessageBox::warning( this, trUtf8( "Attention" ),
                                            trUtf8( "Certains champs n'ont pas été remplis. Ils seront "
                                                   "mis à 0. Voulez-vous vraiment terminer la séance ?" ),
                                            QMessageBox::Yes, QMessageBox::No );
		
        if ( r == QMessageBox::No ) {
            return;
        }
    }
	
    // Persist
    const bool transaction = SqlHelper::transaction();
    bool ok = true;
	
    foreach ( ExerciseWidget* ew, mExercises ) {
        if ( !ew->save( mUserId, mSessionId ) )
        {
            if ( transaction ) {
                SqlHelper::rollback();
            }
			
            ok = false;
            break;
        }
    }

    if ( ok ) {
        QSqlQuery query = SqlHelper::query();
		
        query.prepare( "INSERT INTO session_made(id_session, id_user, date) VALUES(:id_session, :id_user, :date)" );
        query.bindValue( ":id_session", mSessionId );
        query.bindValue( ":id_user", mUserId );
        query.bindValue( ":date", QDate::currentDate() );
		
        if ( !query.exec() ) {
            if ( transaction ) {
                SqlHelper::rollback();
                ok = false;
            }
        }
    }
	
    if ( transaction && ok ) {
        SqlHelper::commit();
    }
	
    if ( ok ) {
        QMessageBox::information( this, trUtf8( "Information" ),
                                 trUtf8( "La séance du %1 a bien été enregistrée." )
                                 .arg( QDate::currentDate().toString( Qt::SystemLocaleLongDate ) ) );
        emit sessionFinished();
    }
    else  {
        QMessageBox::critical( this, trUtf8( "Erreur" ),
                              trUtf8( "Erreur lors de l'enregistrement de la séance: %1" )
                              .arg( SqlHelper::lastError() ) );
    }*/
}

void SessionIconView::selectExercises()
{
    QSqlQuery query = SqlHelper::query();
	
    query.prepare( "SELECT se.id_exercise, e.name, e.type, e.difficulty, e.weight, e.description, "
                  "se.id_session_exercise, se.rest, se.repetitions, se.series FROM "
                  "session_exercise se INNER JOIN exercise e "
                  "ON se.id_exercise = e.id_exercise "
                  "WHERE id_session=:sessionId" );
    query.bindValue( ":sessionId", mSessionId );
	
    if ( query.exec() ) {
		ExerciseWidgetDataList widgetsData;
		
        while ( query.next() ) {
            ExerciseWidgetData data;
			
			data.exerciseId = query.value( 0 ).toLongLong();
            data.name = query.value( 1 ).toString();
            data.type = Exercise::Type( query.value( 2 ).toInt() );
            data.difficulty = Exercise::Difficulty( query.value( 3 ).toInt() );
            data.weight = query.value( 4 ).toBool();
			data.description = query.value( 5 ).toString();
			data.sessionExerciseId = query.value( 6 ).toLongLong();
            data.rest = query.value( 7 ).toInt();
            data.repetitions = query.value( 8 ).toInt();
            data.series = query.value( 9 ).toInt();
			data.seriesData.reserve( data.series );
			
            widgetsData << data;
        }
		
		mModel->setWidgetsData( widgetsData );
    }
}
