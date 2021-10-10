#include "widget.h"
#include"dialoglist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_MAC_WANTS_LAYER", "1");
    Dialoglist d;
    d.show();
    return a.exec();
}
