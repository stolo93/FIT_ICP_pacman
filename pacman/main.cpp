#include "GUI/main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacmanMainWindow main_window;

    main_window.setMinimumSize(640, 480);
    main_window.setWindowTitle("Pacman");

    main_window.show();
    return QApplication::exec();
}
