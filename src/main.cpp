#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"
#include "exercisesdialog.h"
#include "sessionsdialog.h"
#include "mensurationsdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    UsersDialog dialog;
    //dialog.show();


    /*
    ExercisesDialog exercises;
    exercises.show();

    
    SessionsDialog sessions;
    sessions.show();
    */

    MensurationsDialog mensurations;
    mensurations.show();

    return a.exec();
}
