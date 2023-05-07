/**
 * @file controller.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "controller.h"

#include "../GameState/parse.h"

#include <QObject>
#include <QTimer>
#include <atomic>
#include <fstream>
#include <iostream>

namespace ctl
{
Controller::Controller(QObject *parent)
    : QObject(parent), key_event_queue(std::make_unique<UserKeyEventQueue>(ctl::QUEUE_CAPACITY)),
      player_move(game::Pos()), is_in_update()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Controller::on_timer_timeout);
    timer->start(15);    // 15 ms to achieve a move speed of 64 pixels a second based on some research the original
                         // pacman was 88 pixels a second but this felt uncontrollable
}

Controller::~Controller()
{
    game_states.clear();

    if (log_file.is_open()) { log_file.close(); }
}

void Controller::connect_main_window(const view::PacmanMainWindow *main_window) const
{
    // Connect main window signals to controller slots
    connect(main_window, &view::PacmanMainWindow::start_game, this, &ctl::Controller::on_start_game);
    connect(main_window, &view::PacmanMainWindow::start_replay, this, &ctl::Controller::on_start_replay);
    connect(main_window, &view::PacmanMainWindow::user_event, this, &ctl::Controller::on_user_event);
    connect(main_window, &view::PacmanMainWindow::set_controller_state, this,
            &ctl::Controller::on_set_controller_state);

    // Connect controller signals to main window slots
    connect(this, &Controller::update_game_screen, main_window, &view::PacmanMainWindow::on_update_game_screen);
    connect(this, &Controller::init_game_screen, main_window, &view::PacmanMainWindow::on_init_game_screen);
    connect(this, &Controller::end_game, main_window, &view::PacmanMainWindow::on_end_game);
}

void Controller::on_timer_timeout()
{
    switch (this->state) {
        case ControllerState::StateNotSetup: {
            return;
        }

        case ControllerState::StatePaused: {
            update_pause_state();
            return;
        }

        case ControllerState::StateGameplay: {
            // Inserts a new game state into game states vector and sets the current_game_state correctly
            update_gameplay_state();
        }

        case ControllerState::StateReplay: {
            // Sets the current_game_state correctly
            update_replay_state();
        }
    }
    emit update_game_screen(std::atomic_load(&current_game_state));
}

void Controller::on_start_game(const std::string &user, const std::string &map_file_name)
{
    // Check if a file is already opened
    if (this->log_file.is_open()) { this->log_file.close(); }

    // Check if the username and the file name are not empty strings
    if (user.empty() || map_file_name.empty()) {
        std::cerr << "Error: user_name = \"" << user << "\" map file name = \"" << map_file_name << "\"" << std::endl;
        return;
    }
    // Try to create a log file
    if (! create_log_file(user, map_file_name)) { return; }

    // Try to parse the log file for map and initial state
    auto map_state_pair = game::parse_map_from_stream(this->log_file);
    if (! map_state_pair.has_value()) {
        this->log_file.close();
        std::cerr << "Error: could not correctly parse the file" << std::endl;
        return;
    }

    // Get the map and the initial state
    this->game_map = map_state_pair->first;
    auto initial_game_state = std::make_shared<game::GameState>(map_state_pair->second);
    std::atomic_store(&this->current_game_state, initial_game_state);

    // Insert initial game state to the game state vector
    this->game_states.push_back(current_game_state);
    current_game_state_idx = 0;

    // Start game play
    this->state = ControllerState::StateGameplay;
    emit this->init_game_screen(std::atomic_load(&current_game_state));
    std::cerr << "Info: Initialized new game" << std::endl;
}

void Controller::on_start_replay(const std::string &log_file_name)
{
    this->log_file = std::fstream(log_file_name);
    if (! log_file.is_open()) {
        std::cerr << "Error: Could not open the log file" << std::endl;
        return;
    }

    // Try to parse the file to get the map and the first state
    auto map_state_pair = game::parse_map_from_stream(this->log_file);
    if (! map_state_pair.has_value()) {
        this->log_file.close();
        std::cerr << "Error: could not correctly parse the file" << std::endl;
        return;
    }

    // Store the map, get first game state and store it as current game_state
    this->game_map = map_state_pair->first;
    auto initial_state = std::make_shared<game::GameState>(map_state_pair->second);
    std::atomic_store(&this->current_game_state, initial_state);

    this->game_states.push_back(this->current_game_state);
    this->current_game_state_idx = 0;

    // Load all other game states into the state while (true)
        while (true) {
            auto current_state = game::parse_state_from_stream(this->log_file, this->game_map);
            if (! current_state.has_value()) { break; }
            this->game_states.push_back(std::make_shared<game::GameState>(current_state.value()));
        }

    this->state = ControllerState::StateReplay;
    emit this->init_game_screen(std::atomic_load(&current_game_state));
    std::cout << "Info: Initialized game replay" << std::endl;
}

void Controller::on_set_controller_state(ControllerState new_state)
{
    if (state == ControllerState::StateReplay && new_state == ControllerState::StateGameplay){
        std::cerr << "Info: replay -> play : state_count = " << game_states.size() << ", index = " << current_game_state_idx << std::endl;        // If current game state index is smaller than number of states, trim state vector
        game_states.resize(current_game_state_idx+1);
        set_current_game_state();
    }
    this->state = new_state;
}

void Controller::on_user_event(QKeyEvent *event)
{
    if (event->type() == QKeyEvent::KeyPress) {
        this->key_event_queue->push(KeyEvent {ctl::KeyEventType::KeyPress, static_cast<Qt::Key>(event->key())});

    } else if (event->type() == QKeyEvent::KeyRelease) {
        this->key_event_queue->push(KeyEvent {ctl::KeyEventType::KeyRelease, static_cast<Qt::Key>(event->key())});
    }
}

void Controller::update_gameplay_state()
{
    // If we're updating the game state just ignore this update. Otherwise, we would suffer from rollbacks.
    // try_lock may spuriously fail even if the mutex is unlocked in this case we still skip the update.
    // Better than possibly rolling back progress. std::lock_guard hasn't been used since it doesn't allow us to use
    // try_lock
    if (! is_in_update.try_lock()) { return; }

    static const auto STEP {game::FixedPointNum<int64_t, 3>(1) / 8};
    static const auto NEGATIVE_STEP {game::FixedPointNum<int64_t, 3>(-1) / 8};

    KeyEvent key_event {};

    // Process key events
    while (! this->key_event_queue->empty()) {
        this->key_event_queue->pop(key_event);

        if (key_event.event_type == ctl::KeyEventType::KeyRelease) {
            switch (key_event.key) {
                case Qt::Key_Left:
                case Qt::Key_Right:
                    player_move.x = 0;
                    break;
                case Qt::Key_Up:
                case Qt::Key_Down:
                    player_move.y = 0;
                    break;
                default:
                    continue;
            }
        } else {
            switch (key_event.key) {
                case Qt::Key_Left:
                    player_move.x = NEGATIVE_STEP;
                    break;
                case Qt::Key_Right:
                    player_move.x = STEP;
                    break;
                case Qt::Key_Up:
                    player_move.y = NEGATIVE_STEP;
                    break;
                case Qt::Key_Down:
                    player_move.y = STEP;
                    break;
                default:
                    continue;
            }
        }
    }

    // Make the move
    auto move_vector = player_move;
    auto new_game_state = this->current_game_state->update(move_vector);

    this->game_states.push_back(std::make_shared<game::GameState>(new_game_state));
    ++this->current_game_state_idx;

    set_current_game_state();
    is_in_update.unlock();
    if (current_game_state->has_won() || current_game_state->has_lost()){
        std::cerr << "Info: game end, number of states: " << game_states.size() << std::endl;
        log_states();
        this->state = ControllerState::StateNotSetup;
        emit end_game(current_game_state->has_won() ? GameResult::Win : GameResult::Loose);
    }
}

void Controller::update_replay_state()
{
    KeyEvent key_event {};
    while (! this->key_event_queue->empty()) {
        this->key_event_queue->pop(key_event);
        // Ignore key release
        if (key_event.event_type == KeyEventType::KeyRelease) { continue; }

        switch (key_event.key) {
            case Qt::Key_Right:
                std::cerr << "Info: replay: right key" << std::endl;
                this->current_game_state_idx += 15;
                break;
            case Qt::Key_Left:
                std::cerr << "Info: replay: left key" << std::endl;
                this->current_game_state_idx -= 15;
                break;
            default:
                break;
        }

        std::cerr << "Info: current game state idx = " << current_game_state_idx << std::endl;
    }
    set_current_game_state();
}

void Controller::update_pause_state()
{
    KeyEvent event {};
    // clear input queue
    while (! this->key_event_queue->empty()) { this->key_event_queue->pop(event); }
}

void Controller::set_current_game_state()
{
    // If current_game_state index is out of bounds correct it
    if (this->current_game_state_idx >= this->game_states.size()) {
        this->current_game_state_idx = this->game_states.size() - 1;
    } else if (this->current_game_state_idx < 0) {
        this->current_game_state_idx = 0;
    }

    std::atomic_store(&this->current_game_state, this->game_states[current_game_state_idx]);
}

bool Controller::create_log_file(const std::string &user, const std::string &map_file_name)
{
    // Open map file
    std::fstream map_file(map_file_name, std::ios::in);
    if (! map_file.is_open()) {
        std::cerr << "Error: could not open specified map file\n";
        return false;
    }

    // Create log file
    this->log_file = std::fstream(user + ".log", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    if (! log_file.is_open()) {
        std::cerr << "Error: could not create log file\n";
        map_file.close();
        return false;
    }

    // Copy map to the log file
    std::string line {};
    while (std::getline(map_file, line)) {
        line += '\n';
        log_file << line;
    }

    // Move the file pointer to the beginning
    log_file.seekg(0, std::ios::beg);

    map_file.close();
    return true;
}

void Controller::log_states() {
    for (const auto& state : this->game_states){
        game::write_state_to_stream(this->log_file, *state);
    }
}
}    // namespace ctl
