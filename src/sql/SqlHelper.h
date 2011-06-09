#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace SqlHelper
{
	bool openDatabase();
	bool initializeDatabase();
	void closeDatabase();
	
	bool transaction();
	bool commit();
	bool rollback();
	
	QStringList initializeScripts();
	QString connectionName();
	QString databaseLocalFilePath();
	QSqlDatabase database();
	QSqlQuery query();
	QString lastError( const QSqlQuery& query = QSqlQuery(), const char* function = 0 );
	bool executeSqlScript( const QString& filePath );
};

#endif // SQLHELPER_H
