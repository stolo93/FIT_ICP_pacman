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
#include <QResizeEvent>
#include <memory>
#include <vector>

namespace view
{
/**
 * @class GameView
 * @brief Graphics view class which holds and maintains the game scene
 */
class GameView : public QGraphicsView
{

    Q_OBJECT

public:
    explicit GameView(QGraphicsView *parent = nullptr);

signals:

    /**
     * Emit user event to the controller
     * @param event
     */
    void user_event(QKeyEvent *event);


public slots:

    /**
     * Handle initialization of the game screen
     * @param game_state
     */
    void on_init_scene(std::shared_ptr<game::GameState> game_state);

    /**
     * Handle game screen updating
     * @param game_state
     */
    void on_update_scene(std::shared_ptr<game::GameState> game_state);

protected:
    /**
     * Setup scene
     * @param map_width
     * @param map_height
     * @param border_size
     */
    void setup_scene(uint32_t map_width, uint32_t map_height, uint8_t border_size = 1);

    /**
     * Create bound along the map
     * @param margin
     */
    void create_scene_bounds(uint8_t margin);

    /**
     * Initialize game elements which will later be added to the scene
     * @param game_state
     */
    void initialize_game_elements(game::GameState *game_state);

    /**
     * Handle key press
     * @param event
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * Handle key release
     * @param event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

    /**
     * Handle resize event
     * @param event
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    /**
     * Game graphics scene
     */
    std::unique_ptr<QGraphicsScene> scene;

    /**
     * Game element: player
     */
    GraphicsItemPacman *player {};

    /**
     * Game elements: ghosts
     */
    std::vector<GraphicsItemGhost *> ghosts {};

    /**
     * Game elements: keys
     */
    std::vector<GraphicsItemKey *> keys {};


    /**
     * Insert walls and target to the scene
     * @param map
     */
    void insert_map_objects_to_scene(const game::Map &map);
};
}    // namespace view


#endif    // PACMAN_GAMEVIEW_H
