#include <QtGui/QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QSettings>

#include <QErrorMessage>

#include "settings.h"

#include "sql/SqlHelper.h"
#include "usersdialog.h"
#include "mainwindow.h"
#include "graphicsdialog.h"

#define EXIT_SQL_OPEN_ERROR 1
#define EXIT_LOGIN_ERROR 2

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    app.setOrganizationName( "BodEasy" );
    app.setApplicationName( "BodEasy" );
    
    if ( !SqlHelper::openDatabase() ) {
        return EXIT_SQL_OPEN_ERROR;
    }

    GraphicsDialog graphics;
    graphics.show();


    QSettings settings( app.organizationName(), app.applicationName() );
    const QString styleName = settings.value( SETTING_VIEW_STYLE, QString() ).toString();
    if ( !styleName.isEmpty() )
    {
        app.setStyle( styleName );
    }
    const bool autoStart = settings.value( SETTING_AUTO_STARTUP, false ).toBool();
    qint64 userId;
    if ( !autoStart )
    {
    
        UsersDialog dialog;
        if ( dialog.exec() == QDialog::Rejected ) {
            return EXIT_LOGIN_ERROR;
        }
        userId = dialog.userId();
    }
    else
    {
        userId = settings.value( SETTING_LASTUSERID, -1 ).toLongLong();
    }

    MainWindow mw;
    mw.setWindowTitle( app.applicationName() );
    mw.setUserId( userId );
    //mw.resize( 800, 600 );
    mw.show();



    const int result = app.exec();
    SqlHelper::closeDatabase();
    return result;
}
