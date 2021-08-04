#include "mcserverhelper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MCServerHelper w;
    w.show();
    return a.exec();
}
