/**
 * @file screen_game.h
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_SCREENGAME_H
#define PACMAN_SCREENGAME_H

#include "game_view.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <memory>

namespace view {
    class ScreenGame : public QWidget {
    Q_OBJECT

    public:
        explicit ScreenGame(QWidget *parent);

    signals:

        void user_event(QKeyEvent *event);

        void set_state_play();

        void set_state_replay();

        void set_state_pause();

        void init_scene(std::shared_ptr<game::GameState> game_state);

        void update_scene(std::shared_ptr<game::GameState> game_state);

    public slots:

        void on_init_scene(std::shared_ptr<game::GameState> game_state);

        void on_update_scene(std::shared_ptr<game::GameState> game_state);

    private slots:

        void on_user_event(QKeyEvent *event);

        void on_click_play();

        void on_click_pause();

        void on_click_replay();

    private:
        std::unique_ptr<GameView> graphics_view{};

    };
}


#endif //PACMAN_SCREENGAME_H
