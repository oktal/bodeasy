#include <QtGui/QApplication>
#include "usersdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UsersDialog w;
    w.show();

    return a.exec();
}
