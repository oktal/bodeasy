#include <QtGui/QApplication>
#include "usersdialog.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    UsersDialog w;
    w.show();

    return a.exec();
}
