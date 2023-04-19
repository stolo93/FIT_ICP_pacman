#include "pacmanwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacManWindow w;
    w.show();
    return a.exec();
}
