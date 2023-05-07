/**
 * @file game_view.cpp
 * @author Samuel Stolarik
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#include "game_view.h"
#include "../GraphicItems/pacman_graphics_item.h"

#include <iostream>

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
        case Qt::Key::Key_P:
        case Qt::Key::Key_R:
        case Qt::Key::Key_Escape:
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

    const uint8_t border_size = 1;
    setup_scene(map_width, map_height, border_size);

    insert_map_objects_to_scene(*(game_state->map));
    // Create elements and add them to the scene
    initialize_game_elements(game_state.get());
    // Set position of elements
    on_update_scene(game_state);

    create_scene_bounds(border_size);
    setViewportMargins(border_size, border_size, border_size, border_size);
    setScene(scene.get());
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void GameView::on_update_scene(std::shared_ptr<game::GameState> game_state)
{
    qreal pos_x{game_state->player.position.x.to_floating_point<qreal>()};
    qreal pos_y{game_state->player.position.y.to_floating_point<qreal>()};
    player->setPos(pos_x, pos_y);

    for (int i = 0; i < ghosts.size(); i++){
        pos_x = game_state->ghosts[i].position.x.to_floating_point<qreal>();
        pos_y = game_state->ghosts[i].position.y.to_floating_point<qreal>();
        ghosts[i]->setPos(pos_x, pos_y);
    }

    // If some of the keys have been collected remove them
    if (keys.size() != game_state->keys.size()) {
        for (const auto &key: keys) {
            scene->removeItem(key);
            delete key;
        }
        keys.clear();
        for (int i = 0; i < game_state->keys.size(); i++) {
            auto new_key = new GraphicsItemKey();
            scene->addItem(new_key);
            keys.push_back(new_key);
        }
    }


    for (int i = 0; i < keys.size(); i++){
        pos_x = game_state->keys[i].x.to_floating_point<qreal>();
        pos_y = game_state->keys[i].y.to_floating_point<qreal>();
        keys[i]->setPos(pos_x, pos_y);
    }
}

void GameView::initialize_game_elements(game::GameState *game_state) {
    player = new GraphicsItemPacman();
    scene->addItem(player);

    for (int i = 0; i < game_state->ghosts.size(); i++){
        auto new_ghost = new GraphicsItemGhost();
        scene->addItem(new_ghost);
        ghosts.push_back(new_ghost);
    }

    for (int i = 0; i < game_state->keys.size(); i++){
        auto new_key = new GraphicsItemKey();
        scene->addItem(new_key);
        keys.push_back(new_key);
    }
}

void GameView::insert_map_objects_to_scene(const game::Map &map)
{
    for (int i = 0; i < map.get_width(); i++){
        for (int j = 0; j < map.get_height(); j++){
            switch (map.get_map()[j][i]) {
                case game::MapElement::Wall:{
                    auto new_wall = new GraphicsItemWall();
                    new_wall->setPos(i, j);
                    scene->addItem(new_wall);
                    break;
                }
                case game::MapElement::End:{
                    auto new_end = new GraphicsItemTarget();
                    new_end->setPos(i,j);
                    scene->addItem(new_end);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void GameView::create_scene_bounds(uint8_t margin) {
    qreal x = scene->sceneRect().left() - margin;
    qreal y = scene->sceneRect().top() - margin;
    qreal width = scene->sceneRect().width() + 2 * margin;
    qreal height = scene->sceneRect().height() + 2 * margin;

    scene->setSceneRect(x, y, width, height);

    const auto margin_color = Qt::black;
    QPen pen(margin_color, 1);
    scene->addRect(scene->sceneRect(), pen);
}

void GameView::setup_scene(uint32_t map_width, uint32_t map_height, uint8_t border_size){
    scene->setBackgroundBrush(QColor(Qt::darkGray));
    // Offset the position and size by half the pen width
    auto *scene_bounds = new QGraphicsRectItem(-border_size / 2.0, -border_size / 2.0, map_width + border_size, map_height + border_size);

    QPen border_pen(Qt::black);
    border_pen.setWidth(border_size);
    scene_bounds->setPen(border_pen);
    scene->addItem(scene_bounds);
}

void GameView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    if (scene){
        fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
}    // namespace view
