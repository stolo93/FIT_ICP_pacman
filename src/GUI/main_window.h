/**
 * @file main_window.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_MAIN_WINDOW_H
#define PACMAN_MAIN_WINDOW_H

#include "../GameState/GameState.h"
#include "../pacman.h"
#include "ScreenGame/screen_game.h"
#include "ScreenLoadGame/screen_load_game.h"
#include "ScreenMenu/screen_menu.h"
#include "ScreenNewGame/screen_new_game.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>
#include <string>

/**
 * @namespace view
 * @brief Everything contained in the GUI and game view
 */
namespace view
{
/**
 * @class ScreenNumber
 * @brief Screen Number enumeration
 */
enum class ScreenNumber { MainMenu = 0, NewGame = 1, LoadGame = 2, GameScreen = 3, EndScreen = 4 };

/**
 * @class PacmanMainWindow
 * @brief The wholesome main window, containing all necessary screens
 */
class PacmanMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PacmanMainWindow(QMainWindow *parent = nullptr);

signals:

    /**
     * Emit start game
     * @param user_name
     * @param map_file_name
     */
    void start_game(const std::string &user_name, const std::string &map_file_name);

    /**
     * Emit start replay
     * @param log_file_name
     */
    void start_replay(const std::string &log_file_name);

    /**
     * Emit user event
     * @param event
     */
    void user_event(QKeyEvent *event);

    /**
     * Emit set controller state
     * @param state
     */
    void set_controller_state(ctl::ControllerState state);

    /**
     * Emit update scene
     * @param game_state
     */
    void update_scene(std::shared_ptr<game::GameState> game_state);

    /**
     * Emit initialize scene
     * @param game_state
     */
    void init_scene(std::shared_ptr<game::GameState> game_state);

    /**
     * Send text to the end screen label
     * @param text
     */
    void label_set_text(const QString &text);

public slots:

    /**
     * Update game screen
     * From controller
     * @param game_state
     */
    void on_update_game_screen(const std::shared_ptr<game::GameState> game_state);

    /**
     * Display game screen widget and initialize game screen
     * From controller
     * @param game_state
     */
    void on_init_game_screen(const std::shared_ptr<game::GameState> game_state);

    /**
     * Display end screen widget
     * @param result
     */
    void on_end_game(ctl::GameResult result);

private slots:

    /**
     * From menu screen
     */
    void on_goto_new_game_screen();

    /**
     * From menu screen
     */
    void on_goto_load_game_screen();

    /**
     * From new game screen and from load game screen
     */
    void on_goto_menu_screen();

    /**
     * From new game screen
     * Forward to the controller
     * @param user_name
     * @param file_name_map
     */
    void on_start_game(std::string &user_name, std::string &file_name_map);

    /**
     * From load game screen
     * Forward to the controller
     * @param file_name_map
     */
    void on_replay_game(std::string &file_name_map);

    /**
     * Forward key event to the controller
     * @param event
     */
    void on_user_event(QKeyEvent *event);

    /**
     * Forward game state change to the controller
     * @param state
     */
    void on_set_controller_state(ctl::ControllerState state);

private:
    /**
     * All screens stacked
     */
    QStackedWidget *screens;
};
}    // namespace view


#endif    // PACMAN_MAIN_WINDOW_H
