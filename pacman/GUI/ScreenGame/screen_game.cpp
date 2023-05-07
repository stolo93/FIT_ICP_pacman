/**
 * @file screen_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "screen_game.h"

#include <QGraphicsView>
#include <QGridLayout>
#include <QPushButton>

namespace view
{
ScreenGame::ScreenGame(QWidget *parent) : QWidget(parent), graphics_view(std::make_unique<GameView>())
{
    // Create layout for game screen
    auto layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 1);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 10);
    layout->setRowStretch(2, 1);

    // Create buttons and connect everything
    auto play_button = new QPushButton("Play");
    auto pause_button = new QPushButton("Pause");
    auto replay_button = new QPushButton("Replay");

    connect(play_button, &QPushButton::clicked, this, &ScreenGame::on_click_play);
    connect(pause_button, &QPushButton::clicked, this, &ScreenGame::on_click_pause);
    connect(replay_button, &QPushButton::clicked, this, &ScreenGame::on_click_replay);

    connect(this, &ScreenGame::init_scene, graphics_view.get(), &GameView::on_init_scene);
    connect(this, &ScreenGame::update_scene, graphics_view.get(), &GameView::on_update_scene);

    connect(graphics_view.get(), &GameView::user_event, this, &ScreenGame::on_user_event);
    
    auto buttons = new QWidget();
    auto buttons_layout = new QHBoxLayout(buttons);
    buttons_layout->addWidget(pause_button);
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(replay_button);

    layout->addWidget(buttons, 0, 1);
    layout->addWidget(graphics_view.get(), 1, 1);
}

void ScreenGame::on_init_scene(std::shared_ptr<game::GameState> game_state)
{
    emit this->init_scene(std::move(game_state));
}

void ScreenGame::on_update_scene(std::shared_ptr<game::GameState> game_state)
{
    emit this->update_scene(std::move(game_state));
}

void ScreenGame::on_user_event(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key::Key_Left:
        case Qt::Key::Key_Right:
        case Qt::Key::Key_Up:
        case Qt::Key::Key_Down:
            emit this->user_event(event);
            break;
        case Qt::Key::Key_P:
            emit this->set_controller_state(ctl::ControllerState::StateGameplay);
            break;
        case Qt::Key::Key_R:
            emit this->set_controller_state(ctl::ControllerState::StateReplay);
            break;
        case Qt::Key::Key_Escape:
            emit this->set_controller_state(ctl::ControllerState::StatePaused);
            break;
        default:
            break;
    }
}

void ScreenGame::on_click_play()
{
    emit this->set_controller_state(ctl::ControllerState::StateGameplay);
}

void ScreenGame::on_click_pause()
{
    emit this->set_controller_state(ctl::ControllerState::StatePaused);
}

void ScreenGame::on_click_replay()
{
    emit this->set_controller_state(ctl::ControllerState::StateReplay);
}
}    // namespace view
