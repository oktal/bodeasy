#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"
#include "exercisesdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    UsersDialog dialog;
    //dialog.show();
    
    ExercisesDialog exe;
    exe.show();

    return a.exec();
}
