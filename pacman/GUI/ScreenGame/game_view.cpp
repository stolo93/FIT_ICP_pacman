/**
 * @file game_view.cpp
 * @author Samuel Stolarik
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#include "game_view.h"

namespace view {
    GameView::GameView(QGraphicsView *parent) : QGraphicsView(parent), scene(std::make_unique<QGraphicsScene>()) {

    }

    void GameView::keyPressEvent(QKeyEvent *event) {
        // if event->key() is in game related set of keys emit user_event signal, otherwise use
        // QGraphicsScene::keyPressEvent(event) to handle it
    }

    void GameView::keyReleaseEvent(QKeyEvent *event) {
        // if event->key() is in game related set of keys emit user_event signal, otherwise use
        // QGraphicsScene::keyPressEvent(event) to handle it
    }

    void GameView::on_init_scene(std::shared_ptr<game::GameState> game_state) {
        // Insert map to the scene and create all other objects
    }

    void GameView::on_update_scene(std::shared_ptr<game::GameState> game_state) {
        // Change position of objects
    }

}
