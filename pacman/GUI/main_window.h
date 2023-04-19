/**
 * @file main_window.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_MAIN_WINDOW_H
#define PACMAN_MAIN_WINDOW_H

#include "ScreenLoadGame/screen_load_game.h"
#include "ScreenMenu/screen_menu.h"
#include "ScreenNewGame/screen_new_game.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>

enum class ScreenNumber { MainMenu = 0, NewGame = 1, LoadGame = 2 };

class PacmanMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PacmanMainWindow(QMainWindow *parent = nullptr);

private slots:
    void on_goto_new_game_screen();
    void on_goto_load_game_screen();
    void on_goto_home_screen();

private:
    QStackedWidget *screens;
    ScreenMenu *screen_menu = nullptr;
    ScreenNewGame *screen_new_game = nullptr;
    ScreenLoadGame *screen_load_game = nullptr;
};


#endif    // PACMAN_MAIN_WINDOW_H
