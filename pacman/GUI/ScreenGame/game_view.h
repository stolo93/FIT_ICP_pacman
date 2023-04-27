/**
 * @file game_view.h
 * @author Samuel Stolarik
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_GAMEVIEW_H
#define PACMAN_GAMEVIEW_H

#include "../GameState/GameState.h"
#include "../GameState/Map.h"
#include "../GraphicItems/pacman_graphics_item.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <vector>
#include <memory>
#include <QResizeEvent>

namespace view
{
class GameView : public QGraphicsView
{

    Q_OBJECT

public:
    explicit GameView(QGraphicsView *parent = nullptr);

signals:

    void user_event(QKeyEvent *event);


public slots:

    void on_init_scene(std::shared_ptr<game::GameState> game_state);

    void on_update_scene(std::shared_ptr<game::GameState> game_state);

protected:
    void setup_scene(uint32_t map_width, uint32_t map_height, uint8_t border_size = 1);

    void create_scene_bounds(uint8_t margin);

    void initialize_game_elements(game::GameState* game_state);

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<QGraphicsScene> scene;

    // Game elements
    GraphicsItemPacman* player {};
    std::vector<GraphicsItemGhost *> ghosts {};
    std::vector<GraphicsItemKey *> keys {};


    void insert_map_objects_to_scene(const game::Map &map);
};
}    // namespace view


#endif    // PACMAN_GAMEVIEW_H
