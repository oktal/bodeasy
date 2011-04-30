#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    UsersDialog dialog;
    //dialog.show();


    ExercisesDialog exercises;
    exercises.show();

    
    SessionsDialog sessions;
    sessions.show();

    return a.exec();
}
