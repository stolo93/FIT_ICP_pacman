/**
 * @file controller.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "controller.h"
#include "../include/boost/lockfree/queue.hpp"

#include <QObject>
#include <QTimer>
#include <iostream>
#include <fstream>

namespace ctl {
    Controller::Controller(QObject *parent) : QObject(parent), key_press_queue(UserKeyEventQueue()) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Controller::on_timer_timeout);
        timer->start(11); // 11 to achieve approximately 88 updates per second as in original PacMan game
    }

    Controller::~Controller() {
        for (auto game_state: this->game_states) {
            delete game_state;
        }
        game_states.clear();

        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void Controller::on_timer_timeout() {
        switch (this->state) {
            case ControllerState::StateNotSetup: {
                return;
            }

            case ControllerState::StateGameplay: {
                update_game_state();
            }

            case ControllerState::StateReplay: {
                update_replay_state();
            }
        }

        emit update_view(*game_states[current_game_state_idx]);
    }

    void Controller::on_start_game(const std::string &user_name, const std::string &map_file_name) {
        if (!create_log_file(user_name, map_file_name)) {
            return;
        }

        // TODO insert initial game state to the game states vector

        this->state = ControllerState::StateGameplay;
    }

    void Controller::on_start_replay(const std::string &log_file_name) {
        this->log_file = std::fstream(log_file_name);
        if (!log_file.is_open()) {
            return;
        }

        this->state = ControllerState::StateReplay;
    }

    void Controller::on_set_state_game() {
        this->state = ControllerState::StateGameplay;
    }

    void Controller::on_set_state_replay() {
        this->state = ControllerState::StateReplay;
    }

    void Controller::on_user_event(QKeyEvent *event) {
        if (event->type() == QKeyEvent::KeyPress) {
            this->key_press_queue.push(KeyEvent{true, static_cast<Qt::Key>(event->key())});

        } else if (event->type() == QKeyEvent::KeyRelease) {
            this->key_press_queue.push(KeyEvent{false, static_cast<Qt::Key>(event->key())});
        }
    }

    void Controller::update_game_state() {
        // Take all unconsumed keyboard events from queue and create
        // a new game state which will be pushed to the game state vector

        // Increment current game state index
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
