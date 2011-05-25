#include "SqlHelper.h"
#include "parser/tosqlparse.h"

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

void printSt(toSQLParse::statement &stat, int level)
{
    for (int i = 0; i < level; i++)
        printf("    ");

    /*switch (stat.StatementClass)
    {
    case toSQLParse::statement::unknown:
        printf("[U]");
        break;
    case toSQLParse::statement::ddldml:
        printf("[D]");
        break;
    case toSQLParse::statement::plsqlblock:
        printf("[P]");
    default:
        printf("[B]");
    }*/

    switch (stat.Type)
    {
    case toSQLParse::statement::Block:
        printf("Block:");
        break;
    case toSQLParse::statement::Statement:
        printf("Statement:");
        break;
    case toSQLParse::statement::List:
        printf("List:");
        break;
    case toSQLParse::statement::Keyword:
        printf("Keyword:");
        break;
    case toSQLParse::statement::Token:
        printf("Token:");
        break;
    /*case toSQLParse::statement::EndOfStatement:
        printf("EndOfStatement:");
        break;*/
    case toSQLParse::statement::Raw:
        printf("Raw:");
        break;
    }
    printf("%s (%d)\n", (const char *)stat.String.toUtf8(), stat.Line);
    if (!stat.Comment.isNull())
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("Comment:%s\n", (const char *)stat.Comment.toUtf8());
    }
    for (std::list<toSQLParse::statement>::iterator i = stat.subTokens().begin();
            i != stat.subTokens().end();
            i++)
        printSt(*i, level + 1);
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
	
	toSQLParse::settings cur;
	cur.ExpandSpaces = false;
	cur.CommaBefore = false;
	cur.BlockOpenLine = false;
	cur.OperatorSpace = false;
	cur.KeywordUpper = false;
	cur.RightSeparator = false;
	cur.EndBlockNewline = false;
	cur.IndentLevel = true;
	cur.CommentColumn = false;
	toSQLParse::setSetting(cur);
	
	const QString content = QString::fromUtf8( file.readAll() );
	toSQLParse::stringTokenizer tokens( content );
	int lastOffset = 0;
	int count = 0;
	
	while ( tokens.offset() != content.length() ) {
		const toSQLParse::statement statement = toSQLParse::parseStatement( tokens );
		const QString sql = content.mid( lastOffset, tokens.offset() -lastOffset ).trimmed();
		
		
		if ( !sql.isEmpty() && !q.exec( sql ) ) {
			SqlHelper::debugQuery( q, Q_FUNC_INFO );
			return false;
		}
		
		lastOffset = tokens.offset();
		count++;
	}
	
	qWarning() << "Count" << count -1;
	return true;
}

void SqlHelper::debugQuery( const QSqlQuery& query, const char* function )
{
	qWarning() << function;
	qWarning() << qPrintable( query.lastError().text() );
	qWarning() << qPrintable( query.lastQuery() );
	qWarning() << qPrintable( query.executedQuery() );
}
