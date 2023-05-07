/**
 * @file screen_game.h
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_SCREENGAME_H
#define PACMAN_SCREENGAME_H

#include "../pacman.h"
#include "game_view.h"
#include "livesdisplayimage.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>
#include <memory>

namespace view
{
class ScreenGame : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenGame(QWidget *parent);

signals:

    /**
     * Emit on user event
     * @param event
     */
    void user_event(QKeyEvent *event);

    /**
     * Set controller state
     * @param state
     */
    void set_controller_state(ctl::ControllerState state);

    /**
     * Initialize the scene
     * @param game_state
     */
    void init_scene(std::shared_ptr<game::GameState> game_state);

    /**
     * Update scene
     * @param game_state
     */
    void update_scene(std::shared_ptr<game::GameState> game_state);

public slots:

    /**
     * Initialize the scene
     * @brief Forward signal to the game screens
     * @param game_state
     */
    void on_init_scene(std::shared_ptr<game::GameState> game_state);

    /**
     * Update the scene
     * @brief Forward signal to the game screens
     * @param game_state
     */
    void on_update_scene(std::shared_ptr<game::GameState> game_state);

private slots:

    /**
     * Handle user event
     * @param event
     */
    void on_user_event(QKeyEvent *event);

    /**
     * Handle click play
     */
    void on_click_play();

    /**
     * Handle click pause
     */
    void on_click_pause();

    /**
     * Handle click replay
     */
    void on_click_replay();

private:
    /**
     * Game View
     */
    std::unique_ptr<GameView> graphics_view;

    /**
     * Live counter
     */
    LivesDisplayImage *lives_display;
};
}    // namespace view


#endif    // PACMAN_SCREENGAME_H
