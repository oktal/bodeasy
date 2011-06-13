#include "SessionProxy.h"
#include "sql/SqlHelper.h"

#include <QVBoxLayout>
#include <QVariant>
#include <QDate>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

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
    }
}

qint64 SessionProxy::userId() const
{
	return mUserId;
}

qint64 SessionProxy::sessionId() const
{
	return mSessionId;
}

qint64 SessionProxy::sessionMadeId() const
{
	return mSessionMadeId;
}

bool SessionProxy::isRunning() const
{
    return mRunning;
}

SessionProxy::Type SessionProxy::type() const
{
	return mType;
}

bool SessionProxy::isReadOnly() const
{
    return mReadOnly;
}

bool SessionProxy::isModified() const
{
	ExerciseWidgetDataList data;
	QMetaObject::invokeMethod( mWidget, "widgetsData", Q_RETURN_ARG( ExerciseWidgetDataList, data ) );
	return isModified( data );
}

bool SessionProxy::setRunning( bool running, SessionProxy::Type type, bool readOnly, bool askConfirmation )
{
    if ( mRunning == running && mReadOnly == readOnly && mType == type ) {
        return true;
    }
    
    if ( mRunning ) {
        if ( !mReadOnly ) {
			ExerciseWidgetDataList data;
			bool objectiveDone;
			bool ok = QMetaObject::invokeMethod( mWidget, "widgetsData", Q_RETURN_ARG( ExerciseWidgetDataList, data ) )
				&& QMetaObject::invokeMethod( mWidget, "objectiveDone", Q_RETURN_ARG( bool, objectiveDone ) );
			
			if ( ok ) {
				if ( isModified( data ) ) {
					const QString text = trUtf8( "Voulez vous sauvegarder les modifications ?" );
					
					if ( ( askConfirmation
							&& QMessageBox::question( this, QString::null, text, QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
						|| !askConfirmation ) {
						if ( !commit( data, objectiveDone ) ) {
							return false;
						}
					}
				}
			}
			else {
				return false;
			}
        }
    }
    
	mType = type;
    mRunning = running;
    mReadOnly = readOnly;
    
    updateModel();
    
    if ( mRunning ) {
        emit sessionStarted( mType, mReadOnly );
    }
    else {
        emit sessionFinished();
    }
	
	return true;
}

bool SessionProxy::stop( bool askConfirmation )
{
    return setRunning( false, SessionProxy::Unknown, true, askConfirmation );
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
	QMetaObject::invokeMethod( mWidget, "setWidgetsData",
		Q_ARG( ExerciseWidgetDataList, selectExercises() ),
		Q_ARG( const QString&, selectObjective() ),
		Q_ARG( bool, selectObjectiveDone() ),
		Q_ARG( bool, mReadOnly ) );
}

bool SessionProxy::commit( const ExerciseWidgetDataList& data, bool objectiveAchieved )
{
	bool isComplete = true;

	// Check if each exercise is complete
	foreach ( const ExerciseWidgetData& d, data ) {
		if ( !isCompleteExercise( d ) ) {
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
			return false;
		}
	}
	
    // Persist
    const bool transaction = SqlHelper::transaction();
    bool ok = true;
	
	QSqlQuery query = SqlHelper::query();

    query.prepare( "INSERT INTO session_made(id_session, id_user, date, objective_achieved) "
                   "VALUES(:id_session, :id_user, :date, :objectiveAchieved)" );
	query.bindValue( ":id_session", mSessionId );
	query.bindValue( ":id_user", mUserId );
	query.bindValue( ":date", QDateTime::currentDateTime() );
    query.bindValue( ":objectiveAchieved", objectiveAchieved );

	ok = query.exec();
	
	if ( !ok ) {
		emit error( SqlHelper::lastError( query, Q_FUNC_INFO ) );
	}
	else {
		mSessionMadeId = query.lastInsertId().toLongLong();
		
		foreach ( const ExerciseWidgetData& d, data ) {
			if ( !saveExercise( d ) ) {
				ok = false;
				break;
			}
		}
	}
	
    if ( transaction ) {
		if ( ok ) {
			SqlHelper::commit();
		}
		else {
			SqlHelper::rollback();
		}
    }
	
    if ( ok ) {
        QMessageBox::information( this, trUtf8( "Information" ),
                                 trUtf8( "La séance du %1 a bien été enregistrée." )
                                 .arg( QDate::currentDate().toString( Qt::SystemLocaleLongDate ) ) );
    }
    else {
        QMessageBox::critical( this, trUtf8( "Erreur" ),
                              trUtf8( "Erreur lors de l'enregistrement de la séance: %1" )
                              .arg( SqlHelper::lastError( QSqlQuery(), Q_FUNC_INFO ) ) );
    }
	
	if ( ok ) {
		emit sessionCommited( data );
	}
	
	return ok;
}

void SessionProxy::rollback()
{
	if ( mRunning ) {
		mType = SessionProxy::Unknown;
		mRunning = false;
		mReadOnly = true;
		updateModel();
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
                  "WHERE id_session = :sessionId" );
    query.bindValue( ":sessionId", mSessionId );
	
    if ( query.exec() ) {
		int number = 0;
		
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
			data.number = number++;
			
			if ( mType == SessionProxy::SessionMade ) {
				QSqlQuery querySeries = SqlHelper::query();
				querySeries.prepare( "SELECT result, load, serie_number FROM session_made_result_view "
									"WHERE id_session_made=:sessionMadeId AND id_session_exercise=:sessionExerciseId "
									"ORDER BY serie_number" );
				querySeries.bindValue( ":sessionMadeId", mSessionMadeId );
				querySeries.bindValue( ":sessionExerciseId", data.sessionExerciseId );

				if ( querySeries.exec() ) {
					
					while ( querySeries.next() ) {
                        const int result = querySeries.value( 0 ).toInt();
                        const int load = querySeries.value( 1 ).toInt();
                        data.seriesData << qMakePair( result, load );
					}
				}
				else {
					emit const_cast<SessionProxy*>( this )->error( SqlHelper::lastError( querySeries, Q_FUNC_INFO ) );
				}
			}
			
            dataList << data;
        }
    }
    else {
        emit const_cast<SessionProxy*>( this )->error( SqlHelper::lastError( query, Q_FUNC_INFO ) );
    }
    
    return dataList;
}

QString SessionProxy::selectObjective() const
{
    QSqlQuery query = SqlHelper::query();
	
    query.prepare( "SELECT objective FROM session WHERE id_session=:sessionId" );
    query.bindValue( ":sessionId", mSessionId );
	
    if ( query.exec() && query.next() ) {
        return query.value( 0 ).toString();
    }

    return QString::null;
}

bool SessionProxy::selectObjectiveDone() const
{
	if ( mReadOnly ) {
		QSqlQuery query = SqlHelper::query();
		
		query.prepare( "SELECT objective_achieved FROM session_made WHERE id_session_made=:sessionMadeId" );
		query.bindValue( ":sessionMadeId", mSessionMadeId );
		
		if ( query.exec() && query.next() ) {
			return query.value( 0 ).toBool();
		}
	}

    return false;
}

bool SessionProxy::isModified( const ExerciseWidgetDataList& data ) const
{
	foreach ( const ExerciseWidgetData& d, data ) {
		if ( d.modified ) {
			return true;
		}
	}
	
	return false;
}

bool SessionProxy::isCompleteExercise( const ExerciseWidgetData& data ) const
{
	foreach ( const ExerciseWidgetData::PairIntInt& pair, data.seriesData ) {
		if ( pair.first == 0 || ( data.weight && pair.second == 0 ) ) {
			return false;
		}
	}
	
	return true;
}

bool SessionProxy::saveExerciseSeries( qint64 resultId )
{
	QSqlQuery query = SqlHelper::query();
	
	query.prepare( "INSERT INTO session_made_result(id_session_made, id_result) "
				"VALUES(:id_session_made, :id_result)" );
	query.bindValue( ":id_session_made", mSessionMadeId );
	query.bindValue( ":id_result", resultId );
	
	const int ok = query.exec();
	
	if ( !ok ) {
		emit error( SqlHelper::lastError( query, Q_FUNC_INFO ) );
	}
	
	return ok;
}

bool SessionProxy::saveExercise( const ExerciseWidgetData& data )
{
    QSqlQuery query = SqlHelper::query();
	
	query.prepare(
				"INSERT INTO exercise_result( result, load, date, serie_number, id_exercise, id_session, id_session_exercise, id_user ) "
				"VALUES( :result, :load, :date, :serie_number, :id_exercise, :id_session, :id_session_exercise, :id_user )"
			);
    query.bindValue( ":date", QDate::currentDate() );
	query.bindValue( ":id_exercise", data.exerciseId );
    query.bindValue( ":id_session", mSessionId );
	query.bindValue( ":id_session_exercise", data.sessionExerciseId );
    query.bindValue( ":id_user", mUserId );
	
	int serie = 0;
	
	foreach ( const ExerciseWidgetData::PairIntInt& pair, data.seriesData ) {
		query.bindValue( ":serie_number", serie++ );
		query.bindValue( ":result", pair.first );

		if ( !data.weight ) {
			query.bindValue( ":load", QVariant( QVariant::Int ) );
		}
		else {
			query.bindValue( ":load", pair.second );
		}

		if ( !query.exec() ) {
			emit error( SqlHelper::lastError( query, Q_FUNC_INFO ) );
			return false;
		}
		else {
			const qint64 resultId = query.lastInsertId().toLongLong();
			
			if ( !saveExerciseSeries( resultId ) ) {
				return false;
			}
		}
	}
	
	return true;
}
