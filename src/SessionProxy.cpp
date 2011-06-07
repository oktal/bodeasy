#include "SessionProxy.h"
#include "sql/SqlHelper.h"

#include <QVBoxLayout>
#include <QVariant>
#include <QDebug>

SessionProxy::SessionProxy( QWidget* parent )
    : QWidget( parent ),
		mType( SessionProxy::Unknown ),
		mRunning( false ),
		mReadOnly( true ),
		mUserId( -1 ),
		mSessionId( -1 ),
		mSessionMadeId( -1 )
{
    QVBoxLayout* vl = new QVBoxLayout( this );
    vl->setMargin( 0 );
    vl->setSpacing( 0 );
}

QWidget* SessionProxy::widget() const
{
    return mWidget;
}

void SessionProxy::setWidget( QWidget* widget )
{
    if ( mWidget ) {
        if ( mWidget == widget ) {
            return;
        }
        
        delete mWidget;
    }
    
    mWidget = widget;
    
    if ( mWidget ) {
        layout()->addWidget( mWidget );
		connect( mWidget, SIGNAL( commitSession( const ExerciseWidgetDataList& ) ), this, SLOT( commit( const ExerciseWidgetDataList& ) ), Qt::UniqueConnection );
		connect( mWidget, SIGNAL( finishSession() ), this, SIGNAL( sessionFinished() ), Qt::UniqueConnection );
    }
}

bool SessionProxy::isRunning() const
{
    return mRunning;
}

bool SessionProxy::isReadOnly() const
{
    return mReadOnly;
}

void SessionProxy::setRunning( bool running, SessionProxy::Type type, bool readOnly )
{
//qWarning() << Q_FUNC_INFO << running << readOnly << type;
    if ( mRunning == running && mReadOnly == readOnly && mType == type ) {
        return;
    }
    
    if ( mRunning ) {
        if ( !mReadOnly ) {
			#warning FIX ME
			// request user confirmation before commit
            //commit();
        }
    }
    
	mType = type;
    mRunning = running;
    mReadOnly = readOnly;
    
    updateModel();
    
    if ( mRunning ) {
        emit sessionStarted( mReadOnly );
    }
    else {
        emit sessionFinished();
    }
}

void SessionProxy::setUserId( qint64 id )
{
    mUserId = id;
}

void SessionProxy::setSessionId( qint64 id )
{
    mSessionId = id;
}

void SessionProxy::setSessionMadeId( qint64 id )
{
    mSessionMadeId = id;
}

void SessionProxy::updateModel()
{
	QMetaObject::invokeMethod( mWidget, "sessionUpdated", Q_ARG( ExerciseWidgetDataList, selectExercises() ), Q_ARG( bool, mReadOnly ) );
}

void SessionProxy::commit( const ExerciseWidgetDataList& data )
{
	/*
	QList<ExerciseWidget*>::const_iterator it;

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
    }
	*/
	
	emit sessionCommited( data );
}

void SessionProxy::rollback()
{
	if ( mRunning ) {
		mRunning = false;
		emit sessionFinished();
	}
}

ExerciseWidgetDataList SessionProxy::selectExercises() const
{
    ExerciseWidgetDataList dataList;
    
    if ( mSessionId == -1 && mSessionMadeId == -1 ) {
        return dataList;
    }
    
    QSqlQuery query = SqlHelper::query();
	
    query.prepare( "SELECT se.id_exercise, e.name, e.type, e.difficulty, e.weight, e.description, "
                  "se.id_session_exercise, se.rest, se.repetitions, se.series FROM "
                  "session_exercise se INNER JOIN exercise e "
                  "ON se.id_exercise = e.id_exercise "
                  "WHERE id_session=:sessionId" );
    query.bindValue( ":sessionId", mSessionId );
	
    if ( query.exec() ) {
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
			
			if ( mType == SessionProxy::SessionMade ) {
				QSqlQuery querySeries = SqlHelper::query();
				querySeries.prepare( "SELECT result, load, serie_number FROM session_made_result_view "
									"WHERE id_session_made=:sessionMadeId AND id_session_exercise=:sessionExerciseId "
									"ORDER BY serie_number" );
				querySeries.bindValue( ":sessionMadeId", mSessionMadeId );
				querySeries.bindValue( ":sessionExerciseId", data.exerciseId );

				if ( querySeries.exec() ) {
					int index = 0;
					
					while ( querySeries.next() ) {
						data.seriesData[ index ].first = querySeries.value( 0 ).toInt();
						data.seriesData[ index ].second = querySeries.value( 1 ).toInt();
						index++;
					}
				}
				else {
					emit const_cast<SessionProxy*>( this )->error( SqlHelper::lastError() );
				}
			}
			
            dataList << data;
        }
    }
    else {
        emit const_cast<SessionProxy*>( this )->error( SqlHelper::lastError() );
    }
    
    return dataList;
}
