#include <QtGui/QApplication>
#include <QTextCodec>

#include "usersdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UsersDialog dialog;
    dialog.show();

    return a.exec();
}
