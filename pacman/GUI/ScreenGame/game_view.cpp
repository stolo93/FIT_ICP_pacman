/**
 * @file game_view.cpp
 * @author Samuel Stolarik
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#include "game_view.h"

namespace view
{
GameView::GameView(QGraphicsView *parent) : QGraphicsView(parent) {}

void GameView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key::Key_Left:
        case Qt::Key::Key_Right:
        case Qt::Key::Key_Up:
        case Qt::Key::Key_Down:
            emit this->user_event(event);
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key::Key_Left:
        case Qt::Key::Key_Right:
        case Qt::Key::Key_Up:
        case Qt::Key::Key_Down:
            emit this->user_event(event);
            break;
        default:
            QGraphicsView::keyReleaseEvent(event);
            break;
    }
}

void GameView::on_init_scene(std::shared_ptr<game::GameState> game_state)
{
    auto map_width = static_cast<qreal>(game_state->map->get_width());
    auto map_height = static_cast<qreal>(game_state->map->get_height());

    this->scene = std::make_unique<QGraphicsScene>(0, 0, map_width, map_height);

    insert_map_objects_to_scene(*(game_state->map));

    // TODO insert other objects to the scene
}

void GameView::on_update_scene(std::shared_ptr<game::GameState> game_state)
{
    // Change position of objects
}

void GameView::insert_map_objects_to_scene(const game::Map &map)
{
    // TODO insert walls to the scene
}

}    // namespace view
