/**
 * @file controller.h
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_CONTROLLER_H
#define PACMAN_CONTROLLER_H

#include "../GameState/GameState.h"
#include "../include/boost/lockfree/queue.hpp"
#include "../GUI/main_window.h"

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <fstream>
#include <memory>

namespace ctl {

    struct KeyEvent {
        bool pressed;
        Qt::Key key;
    };

    const int queue_capacity = 1000;
    using UserKeyEventQueue = boost::lockfree::queue<KeyEvent>;

    enum class ControllerState {
        StateReplay,
        StateGameplay,
        StatePaused,
        StateNotSetup
    };

    class Controller : public QObject {
    Q_OBJECT

    public:
        explicit Controller(QObject *parent = nullptr);

        ~Controller() noexcept;

        void connect_main_window(const view::PacmanMainWindow *main_window) const;

    signals:

        /**
         * Signal containing game state to be rendered
         * @param game_state
         */
        void update_view(const std::shared_ptr<game::GameState> game_state);

        /**
         * Send initial game state
         * This is separate from the update, because the window needs to load map only this time
         * @param game_state
         */
        void init_game_screen(const std::shared_ptr<game::GameState> game_state);

    public slots:

        /**
         * Start a new game
         * @param user_name
         * @param map_file_name path to the file containing initial map
         */
        void on_start_game(const std::string &user_name, const std::string &map_file_name);

        /**
         * Start replay of the game logged in @p log_file_name
         * @param log_file_name log file
         */
        void on_start_replay(const std::string &log_file_name);

        /**
         * Set controller state to gameplay
         */
        void on_set_state_game();

        /**
         * Set controller state to replay
         */
        void on_set_state_replay();

        /**
         * Set controller state to paused game
         */
        void on_set_state_paused();

        /**
         * Handle an event received by graphics view
         * @param event
         */
        void on_user_event(QKeyEvent *event);

    private slots:

        /**
         * Wake up on timer timeout
         */
        void on_timer_timeout();

    private:
        ControllerState state{ControllerState::StateNotSetup};
        QTimer *timer;
        std::string user_name;
        std::shared_ptr<game::GameState> current_game_state{};
        std::vector<std::shared_ptr<game::GameState>> game_states{};
        int current_game_state_idx{};
        std::unique_ptr<UserKeyEventQueue> key_press_queue;
        std::fstream log_file{};

        /**
         * Handle all user events received since last call and crete next game state
         * Insert newly created game state to the vector of game states and change the game_state_index accordingly
         */
        void update_game_state();

        /**
         * Handle user events received since last call and change the current state index accordingly
         */
        void update_replay_state();

        /**
         * Copy map from @p map_file_name
         * @param user_name
         * @param map_file_name
         * @return
         */
        bool create_log_file(const std::string &user_name, const std::string &map_file_name);
    };
}


#endif //PACMAN_CONTROLLER_H
