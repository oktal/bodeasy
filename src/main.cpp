#include <QtGui/QApplication>

#include "sql/SqlHelper.h"
#include "usersdialog.h"
#include "mainwindow.h"

#define EXIT_SQL_OPEN_ERROR 1
#define EXIT_LOGIN_ERROR 2

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    app.setApplicationName( "BodEasy" );
    
    if ( !SqlHelper::openDatabase() ) {
        return EXIT_SQL_OPEN_ERROR;
    }
    
    UsersDialog dialog;
    if ( dialog.exec() == QDialog::Rejected ) {
        return EXIT_LOGIN_ERROR;
    }

    MainWindow mw( dialog.userId() );
    mw.setWindowTitle( app.applicationName() );
    mw.showMaximized();

    const int result = app.exec();
    SqlHelper::closeDatabase();
    return result;
}
