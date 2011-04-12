#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"

#include "exercisesdialog.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    ExercisesDialog dialog;
    dialog.exec();

    return a.exec();
}
