#include "GUI/main_window.h"
#include "Controller/controller.h"

#include <memory>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto main_window = std::make_unique<view::PacmanMainWindow>();
    auto controller = std::make_unique<ctl::Controller>();

    controller->connect_main_window(main_window.get());
    main_window->setMinimumSize(640, 480);
    main_window->setWindowTitle("Pacman");

    main_window->show();
    return QApplication::exec();
}
