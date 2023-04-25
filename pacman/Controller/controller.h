/**
 * @file controller.h
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_CONTROLLER_H
#define PACMAN_CONTROLLER_H

#include "../GameState/GameState.h"

#include <QObject>
#include <QTimer>
#include <QEvent>
#include <fstream>

namespace ctl {
    enum class ControllerState {
        StateReplay,
        StateGameplay,
        StateNotSetup
    };

    class Controller : public QObject {
    Q_OBJECT

    public:
        explicit Controller(QObject *parent = nullptr);

    signals:

        /**
         * Signal containing game state to be rendered
         * @param game_state
         */
        void update_view(game::GameState &game_state);

    private slots:

        /**
         * Start a new game
         * @param user_name
         * @param map_file_name path to the file containing initial map
         */
        void on_start_game(std::string user_name, std::string map_file_name);

        /**
         * Start replay of the game logged in @p log_file_name
         * @param log_file_name log file
         */
        void on_start_replay(std::string log_file_name);

        /**
         * Set controller state to gameplay
         */
        void on_set_state_game();

        /**
         * Set controller state to replay
         */
        void on_set_state_replay();

        /**
         * Handle an event received by graphics view
         * @param event
         */
        void on_user_event(QEvent *event);

        /**
         * Wake up on timer timeout
         */
        void on_timer_timeout();

    private:
        ControllerState state{ControllerState::StateNotSetup};
        QTimer *timer;
        std::vector<game::GameState> game_states{};
        int current_game_state_idx{};
        // TODO add input QEvent queue
        std::fstream log_file{};

        /**
         * Handle all user events received since last call and crete next game state
         * Insert newly created game state to the vector of game states and change the game_state_index accordingly
         */
        void update_game_state();
    };
}


#endif //PACMAN_CONTROLLER_H
