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
#include "ScreenGame/screen_game.h"
#include "../GameState/GameState.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>
#include <string>

namespace view {
    enum class ScreenNumber {
        MainMenu = 0, NewGame = 1, LoadGame = 2, GameScreen = 3
    };

    class PacmanMainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit PacmanMainWindow(QMainWindow *parent = nullptr);

    signals:

        void start_game(const std::string &user_name, const std::string &map_file_name);

        void start_replay(const std::string &log_file_name);

        void user_event(QKeyEvent *event);

        void set_state_game();

        void set_state_replay();

    public slots:

        void on_update_view(const std::shared_ptr<game::GameState> game_state);

        void on_init_game_screen(const std::shared_ptr<game::GameState> game_state);

    private slots:

        void on_goto_new_game_screen();

        void on_goto_load_game_screen();

        void on_goto_home_screen();

        void on_start_game(std::string &user_name, std::string &file_name_map);

        void on_replay_game(std::string &file_name_map);

    private:
        QStackedWidget *screens;
    };
}


#endif    // PACMAN_MAIN_WINDOW_H

