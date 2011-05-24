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
	
	QStringList initializeScripts();
	QString connectionName();
	QString databaseLocalFilePath();
	QSqlDatabase database();
	QSqlQuery query();
	bool executeSqlScript( const QString& filePath );
	
	void debugQuery( const QSqlQuery& query, const char* function );
};

#endif // SQLHELPER_H
