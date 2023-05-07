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
#include "ScreenEndGame/screenendgame.h"

#include <QFrame>
#include <QGridLayout>
#include <QMainWindow>
#include <QSizeGrip>
#include <QStackedWidget>

namespace view {
    PacmanMainWindow::PacmanMainWindow(QMainWindow *parent) : QMainWindow(parent), screens(new QStackedWidget{this}) {
        auto screen_menu = new ScreenMenu{this};
        auto screen_new_game = new ScreenNewGame{this};
        auto screen_load_game = new ScreenLoadGame{this};
        auto screen_game = new ScreenGame{this};
        auto screen_end = new ScreenEndGame{this};

        connect(screen_menu, &ScreenMenu::go_to_new_game_screen, this, &PacmanMainWindow::on_goto_new_game_screen);
        connect(screen_menu, &ScreenMenu::go_to_load_game_screen, this, &PacmanMainWindow::on_goto_load_game_screen);

        connect(screen_new_game, &ScreenNewGame::go_to_home, this, &PacmanMainWindow::on_goto_menu_screen);
        connect(screen_load_game, &ScreenLoadGame::go_to_home, this, &PacmanMainWindow::on_goto_menu_screen);

        connect(screen_new_game, &ScreenNewGame::start_game, this, &PacmanMainWindow::on_start_game);
        connect(screen_load_game, &ScreenLoadGame::replay_game, this, &PacmanMainWindow::on_replay_game);

        connect(screen_game, &ScreenGame::user_event, this, &PacmanMainWindow::on_user_event);
        connect(screen_game, &ScreenGame::set_controller_state, this, &PacmanMainWindow::on_set_controller_state);
        connect(this, &PacmanMainWindow::init_scene, screen_game, &ScreenGame::on_init_scene);
        connect(this, &PacmanMainWindow::update_scene, screen_game, &ScreenGame::on_update_scene);

        connect(this, &PacmanMainWindow::label_set_text, screen_end, &ScreenEndGame::on_set_text);

        // No better way of doing this apparently
        // Don't change the order, it is bound to indices in enum ScreenNumber
        // Defined in main_window.h
        screens->addWidget(screen_menu);
        screens->addWidget(screen_new_game);
        screens->addWidget(screen_load_game);
        screens->addWidget(screen_game);
        screens->addWidget(screen_end);

        // Set starting screen
        screens->setCurrentIndex(static_cast<int>(ScreenNumber::MainMenu));
        this->setCentralWidget(screens);
    }

    void PacmanMainWindow::on_goto_menu_screen() {
        screens->setCurrentIndex(static_cast<int>(ScreenNumber::MainMenu));
        setCentralWidget(screens);
    }

    void PacmanMainWindow::on_goto_new_game_screen() {
        screens->setCurrentIndex(static_cast<int>(ScreenNumber::NewGame));
        setCentralWidget(screens);
    }

    void PacmanMainWindow::on_goto_load_game_screen() {
        screens->setCurrentIndex(static_cast<int>(ScreenNumber::LoadGame));
        setCentralWidget(screens);
    }

    void PacmanMainWindow::on_start_game(std::string &user_name, std::string &file_name_map) {
        emit this->start_game(user_name, file_name_map);
    }

    void PacmanMainWindow::on_replay_game(std::string &file_name_map) {
        emit this->start_replay(file_name_map);
    }

    void PacmanMainWindow::on_init_game_screen(const std::shared_ptr<game::GameState> game_state) {
        screens->setCurrentIndex(static_cast<int>(ScreenNumber::GameScreen));
        setCentralWidget(screens);

        emit this->init_scene(game_state);
    }

    void PacmanMainWindow::on_update_game_screen(const std::shared_ptr<game::GameState> game_state) {
        emit this->update_scene(game_state);
    }

    void PacmanMainWindow::on_user_event(QKeyEvent *event) {
        emit this->user_event(event);
    }

    void PacmanMainWindow::on_set_controller_state(ctl::ControllerState state) {
        emit this->set_controller_state(state);
    }

    void PacmanMainWindow::on_end_game(ctl::GameResult result) {
        QString text {};
        if (result == ctl::GameResult::Win){
            text = "Congratulations, you won!";
        }
        else{
            text = "You loose, game over!";
        }
        emit this->label_set_text(text);

        screens->setCurrentIndex(static_cast<int>(ScreenNumber::EndScreen));
        setCentralWidget(screens);
    }
}

