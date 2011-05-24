#include "SqlHelper.h"

#include <QDesktopServices>
#include <QDir>
#include <QTemporaryFile>
#include <QSqlError>
#include <QDebug>

bool SqlHelper::openDatabase()
{
	qWarning() << Q_FUNC_INFO << "Using database in" << qPrintable( SqlHelper::databaseLocalFilePath() );
	
	bool open = false;
	
	{
		QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", SqlHelper::connectionName() );
		db.setDatabaseName( SqlHelper::databaseLocalFilePath() );
		open = db.open();
		
		if ( open && db.tables().isEmpty() ) {
			open = SqlHelper::initializeDatabase();
		}
	}
	
	if ( !open ) {
		SqlHelper::closeDatabase();
	}
	
	return open;
}

bool SqlHelper::initializeDatabase()
{
	foreach ( const QString& filePath, SqlHelper::initializeScripts() ) {
		if ( !SqlHelper::executeSqlScript( filePath ) ) {
			return false;
		}
	}
	
	return true;
}

void SqlHelper::closeDatabase()
{
	{
		QSqlDatabase db = SqlHelper::database();
		
		if ( db.isOpen() ) {
			db.close();
		}
	}
	
	QSqlDatabase::removeDatabase( SqlHelper::connectionName() );
}

QStringList SqlHelper::initializeScripts()
{
	const QString path = ":/sql";
	const QStringList entries = QDir( path ).entryList( QStringList( "*.sql" ), QDir::Files | QDir::NoDotDot, QDir::Name );
	QStringList files;
	
	foreach ( const QString& entry, entries ) {
		files << QString( "%1/%2" ).arg( path ).arg( entry );
	}
	
	return files;
}

QString SqlHelper::connectionName()
{
	return QLatin1String( "BodEasy DB" );
}

QString SqlHelper::databaseLocalFilePath()
{
	const QString path = QDesktopServices::storageLocation( QDesktopServices::DataLocation );
	const QString filePath = QString( "%1/database.s3db" ).arg( path );
	
	if ( !QFile::exists( filePath ) ) {
		if ( !QDir().mkpath( path ) ) {
			qWarning() << Q_FUNC_INFO << "Can't create path" << qPrintable( path );
		}
	}
	
	return filePath;
}

QSqlDatabase SqlHelper::database()
{
	return QSqlDatabase::database( SqlHelper::connectionName(), false );
}

QSqlQuery SqlHelper::query()
{
	return QSqlQuery( SqlHelper::database() );
}

bool SqlHelper::executeSqlScript( const QString& filePath )
{
	qWarning() << Q_FUNC_INFO << "Executing" << qPrintable( filePath );
	QSqlQuery q = SqlHelper::query();
	QFile file( filePath );
	
	if ( !file.open( QIODevice::ReadOnly ) ) {
		qWarning() << Q_FUNC_INFO << "Can't open file" << qPrintable( filePath );
		return false;
	}
	
	const bool ok = q.exec( QString::fromUtf8( file.readAll() ) );
	
	if ( !ok ) {
		SqlHelper::debugQuery( q, Q_FUNC_INFO );
		return false;
	}
	
	return true;
}

void SqlHelper::debugQuery( const QSqlQuery& query, const char* function )
{
	qWarning() << function;
	qWarning() << qPrintable( query.lastError().text() );
	qWarning() << qPrintable( query.lastQuery() );
	qWarning() << qPrintable( query.executedQuery() );
}
