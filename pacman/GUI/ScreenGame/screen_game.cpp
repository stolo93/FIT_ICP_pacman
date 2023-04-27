/**
 * @file screen_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "screen_game.h"

#include <QGraphicsView>
#include <QGridLayout>

namespace view {
    ScreenGame::ScreenGame(QWidget *parent) : QWidget(parent) {
        auto layout = new QGridLayout(this);
        layout->setColumnStretch(0, 1);
        layout->setColumnStretch(1, 10);
        layout->setColumnStretch(2, 1);
        layout->setRowStretch(0, 1);
        layout->setRowStretch(1, 10);
        layout->setRowStretch(2, 1);

        // TODO add play, pause and replay buttons and connect them to corresponding slots
        layout->addWidget(graphics_view.get(), 1, 1);
    }

    void ScreenGame::on_init_scene(std::shared_ptr<game::GameState> game_state) {
        emit this->init_scene(std::move(game_state));
    }

    void ScreenGame::on_update_scene(std::shared_ptr<game::GameState> game_state) {
        emit this->update_scene(std::move(game_state));
    }

    void ScreenGame::on_user_event(QKeyEvent *event) {
        emit this->user_event(event);
    }

    void ScreenGame::on_click_play() {
        emit this->set_state_play();
    }

    void ScreenGame::on_click_pause() {
        emit this->set_state_pause();
    }

    void ScreenGame::on_click_replay() {
        emit this->set_state_replay();
    }
}
