#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    UsersDialog dialog;
    //dialog.show();

    MainWindow mainWindow;
    mainWindow.showMaximized();

    return a.exec();
}
