/**
 * @file screen_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#include "screen_game.h"

#include <QGraphicsView>
#include <QGridLayout>

ScreenGame::ScreenGame(QWidget *parent) : QWidget(parent), game_view(new QGraphicsView(nullptr, this)) {
    auto layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 1);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 10);
    layout->setRowStretch(2, 1);

    layout->addWidget(game_view, 1, 1);
}

void ScreenGame::on_set_scene(QGraphicsScene *scene) {
    this->game_scene = scene;
    this->game_view->setScene(this->game_scene);
}