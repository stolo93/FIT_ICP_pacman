/**
 * @file main_window.cpp
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#include "main_window.h"

#include "ScreenLoadGame/screen_load_game.h"
#include "ScreenMenu/screen_menu.h"
#include "ScreenNewGame/screen_new_game.h"

#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>
#include <QSizeGrip>
#include <QStackedWidget>

PacmanMainWindow::PacmanMainWindow(QMainWindow *parent) : QMainWindow(parent), screens(new QStackedWidget {this})
{
    screen_menu = new ScreenMenu {this};
    screen_new_game = new ScreenNewGame {this};
    screen_load_game = new ScreenLoadGame {this};

    connect(screen_menu, &ScreenMenu::go_to_new_game_screen, this, &PacmanMainWindow::on_goto_new_game_screen);
    connect(screen_menu, &ScreenMenu::go_to_load_game_screen, this, &PacmanMainWindow::on_goto_load_game_screen);

    connect(screen_new_game, &ScreenNewGame::go_to_home, this, &PacmanMainWindow::on_goto_home_screen);
    connect(screen_load_game, &ScreenLoadGame::go_to_home, this, &PacmanMainWindow::on_goto_home_screen);

    // No better way of doing this apparently
    // Don't change the order, it is bound to indices in enum ScreenNumber
    // Defined in main_window.h
    screens->addWidget(screen_menu);
    screens->addWidget(screen_new_game);
    screens->addWidget(screen_load_game);

    // Set starting screen
    screens->setCurrentIndex(static_cast<int>(ScreenNumber::MainMenu));
    this->setCentralWidget(screens);
}

void PacmanMainWindow::on_goto_home_screen()
{
    screens->setCurrentIndex(static_cast<int>(ScreenNumber::MainMenu));
    setCentralWidget(screens);
}

void PacmanMainWindow::on_goto_new_game_screen()
{
    screens->setCurrentIndex(static_cast<int>(ScreenNumber::NewGame));
    setCentralWidget(screens);
}

void PacmanMainWindow::on_goto_load_game_screen()
{
    screens->setCurrentIndex(static_cast<int>(ScreenNumber::LoadGame));
    setCentralWidget(screens);
}
