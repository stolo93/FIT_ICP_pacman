/**
 * @file controller.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "controller.h"
#include "../GUI/main_window.h"

#include <QObject>
#include <QTimer>
#include <iostream>
#include <fstream>

namespace ctl {
    Controller::Controller(QObject *parent) : QObject(parent),
                                              key_press_queue(
                                                      std::make_unique<UserKeyEventQueue>(ctl::queue_capacity)) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Controller::on_timer_timeout);
        timer->start(11); // 11 ms to achieve approximately 88 updates per second as in original PacMan game
    }

    Controller::~Controller() {
        game_states.clear();

        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void Controller::connect_main_window(const view::PacmanMainWindow *main_window) const {
        // Connect main window signals to controller slots
        connect(main_window, &view::PacmanMainWindow::start_game, this, &ctl::Controller::on_start_game);
        connect(main_window, &view::PacmanMainWindow::start_replay, this, &ctl::Controller::on_start_replay);
        connect(main_window, &view::PacmanMainWindow::user_event, this, &ctl::Controller::on_user_event);
        connect(main_window, &view::PacmanMainWindow::set_state_game, this, &ctl::Controller::on_set_state_game);
        connect(main_window, &view::PacmanMainWindow::set_state_replay, this, &ctl::Controller::on_set_state_replay);

        // Connect controller signals to main window slots
        connect(this, &Controller::update_view, main_window, &view::PacmanMainWindow::on_update_view);
        connect(this, &Controller::init_game_screen, main_window, &view::PacmanMainWindow::on_init_game_screen);
    }

    void Controller::on_timer_timeout() {
        switch (this->state) {
            case ControllerState::StateNotSetup: {
                return;
            }

            case ControllerState::StatePaused: {
                return;
            }

            case ControllerState::StateGameplay: {
                update_game_state();
            }

            case ControllerState::StateReplay: {
                update_replay_state();
            }
        }
        // TODO emit the signal after game states are really being added to the queue
        // emit update_view(std::atomic_load(&current_game_state));
    }

    void Controller::on_start_game(const std::string &user_name, const std::string &map_file_name) {
        if (!create_log_file(user_name, map_file_name)) {
            return;
        }

        // TODO load map, create initial game, insert it to game state vector and send it to the main window
        this->state = ControllerState::StateGameplay;
        // emit this->init_game_screen(std::atomic_load(&current_game_state))
        std::cout << "init game \n";
    }

    void Controller::on_start_replay(const std::string &log_file_name) {
        this->log_file = std::fstream(log_file_name);
        if (!log_file.is_open()) {
            return;
        }

        // TODO load map, load game states to the state vector and send initial state to the main window

        this->state = ControllerState::StateReplay;
        // TODO send initial game state to main window
        // emit this->init_game_screen(current_game_state.load())
        std::cout << "init replay\n";
    }

    void Controller::on_set_state_game() {
        this->state = ControllerState::StateGameplay;
    }

    void Controller::on_set_state_replay() {
        this->state = ControllerState::StateReplay;
    }

    void Controller::on_set_state_paused() {
        this->state = ControllerState::StatePaused;
    }

    void Controller::on_user_event(QKeyEvent *event) {
        if (event->type() == QKeyEvent::KeyPress) {
            this->key_press_queue->push(KeyEvent{true, static_cast<Qt::Key>(event->key())});

        } else if (event->type() == QKeyEvent::KeyRelease) {
            this->key_press_queue->push(KeyEvent{false, static_cast<Qt::Key>(event->key())});
        }
    }

    void Controller::update_game_state() {
        // Take all unconsumed keyboard events from queue and create
        // a new game state which will be pushed to the game state vector

        // Increment current game state index
        // Store current game state
    }

    void Controller::update_replay_state() {
        // Take unconsumed keyboard events which control game replay and adjust current game index accordingly
    }

    bool Controller::create_log_file(const std::string &user_name, const std::string &map_file_name) {
        // Open map file
        std::fstream map_file(map_file_name, std::ios::in);
        if (!map_file.is_open()) {
            std::cerr << "Error: could not open specified map file\n";
            return false;
        }

        // Create log file
        this->log_file = std::fstream(user_name + ".log", std::ios::in | std::ios::out | std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Error: could not create log file\n";
            map_file.close();
            return false;
        }

        // Copy map to the log file
        log_file << user_name << '\n';
        std::string line{};
        while (std::getline(map_file, line)) {
            log_file << line;
        }

        log_file << std::endl;

        map_file.close();
        return true;
    }
}
