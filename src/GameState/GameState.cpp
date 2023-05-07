/**
 * @file GameState.cpp
 * @author Adam Bezák (xbezak02)
 * @date 2023-05-07
 * Project - ICP - project PacMan
 */

#include "GameState.h"

#include <utility>

/**
 *
 */
const game::FixedPointNum<std::int64_t, 3> ONE_HALF = game::FixedPointNum<std::int64_t, 3>(1) / 2;

namespace game
{
/**
 * Checks if 2 boxes of width and height 1 are overlapping
 * @param box_1 The position of the top left corner of the first box
 * @param box_2 The position of the top left corner of the second box
 * @return true if the boxes are overlapping
 */
bool are_boxes_intersecting(Pos box_1, Pos box_2)
{
    bool is_x_intersecting = box_1.x > box_2.x && box_1.x < (box_2.x + 1);
    bool is_y_intersecting = box_1.y > box_2.y && box_1.y < (box_2.y + 1);

    return is_x_intersecting && is_y_intersecting;
}

/**
 * Checks of a box of width 1 and a circle of diameter 1 are overlapping
 * @param box The position of the top left corner of the box
 * @param circle The position of the top left of the box the circle is contained in
 * @return true if the shapes are overlapping
 */
bool is_box_and_circle_intersecting(Pos box, Pos circle)
{
    // https://stackoverflow.com/a/402010
    auto box_center_x = FixedPointNum<std::int64_t, 6>(box.x) + ONE_HALF;
    auto box_center_y = FixedPointNum<std::int64_t, 6>(box.y) + ONE_HALF;

    auto circle_center_x = FixedPointNum<std::int64_t, 6>(circle.x) + ONE_HALF;
    auto circle_center_y = FixedPointNum<std::int64_t, 6>(circle.y) + ONE_HALF;

    auto collision_vector_x = (circle_center_x - box_center_x).abs();
    auto collision_vector_y = (circle_center_y - box_center_y).abs();
    auto box_half_width = ONE_HALF;

    // The player radius 1/2 tiles - 1 pixel
    auto pacman_radius = ONE_HALF - (FixedPointNum<std::int64_t, 3>(1) / 8);

    if (collision_vector_x > (box_half_width + pacman_radius) ||
        collision_vector_y > (box_half_width + pacman_radius)) {
        return false;
    }

    if (collision_vector_x <= box_half_width || collision_vector_y <= box_half_width) { return true; }

    auto corner_distance_squared = (collision_vector_x - box_half_width) * (collision_vector_x - box_half_width) +
                                   (collision_vector_y - box_half_width) * (collision_vector_y - box_half_width);

    auto debug_helper = pacman_radius * pacman_radius;
    return corner_distance_squared <= pacman_radius * pacman_radius;
}

/**
 * Gets the MapElement at the given position.
 * @param map The map to look into.
 * @param pos The position we're querying.
 * @return The MapElement at the map location. Wall if we're looking out of bounds.
 */
MapElement get_map_element(const QExplicitlySharedDataPointer<Map> &map, Pos pos)
{
    if (pos.x < 0 || pos.x >= map->get_width()) { return MapElement::Wall; }

    if (pos.y < 0 || pos.y >= map->get_height()) { return MapElement::Wall; }

    return map->get_map()[pos.y.floor()][pos.x.floor()];
}

/**
 * Checks if the given coordinate is pointing at wall.
 * @param map The map to look into.
 * @param pos The coordinate we're checking.
 * @return true if the current coordinate is pointing at a wall.
 */
bool is_wall(const QExplicitlySharedDataPointer<Map> &map, Pos pos)
{
    return get_map_element(map, pos) == MapElement::Wall;
}


GameState::GameState(QExplicitlySharedDataPointer<Map> map, GameStatus status, uint64_t state_number,
                     QVector<Ghost> ghosts,
                     Player player, uint8_t player_lives, Pos exit, QVector<Pos> keys)
    : map(std::move(map)), state(status), state_number(state_number), ghosts(std::move(ghosts)), player(player),
      exit(exit), keys(std::move(keys)), player_lives(player_lives) {
}

Pos player_wall_check(Pos player_pos, Pos direction, const QExplicitlySharedDataPointer<Map> &map)
{
    auto new_player_pos = player_pos + direction;

    // The player sprite is shifted one pixel to the left so, we manually shift it for collision checks;
    const auto one_pixel = FixedPointNum<std::int64_t, 3>(1) / 8;


    // Now do wall collisions
    if (direction.x > 0) {

        auto top_left_tile_x = (new_player_pos.x + 1).floor();
        auto top_left_tile_y = (new_player_pos.y).floor();
        auto bottom_left_tile_x = (new_player_pos.x + 1).floor();
        auto bottom_left_tile_y = (new_player_pos.y).ceil();

        if (is_wall(map, Pos(new_player_pos.x + 1, new_player_pos.y.floor()))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_left_tile_x, top_left_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }

        if (is_wall(map, Pos(new_player_pos.x + 1, new_player_pos.y.ceil()))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(bottom_left_tile_x, bottom_left_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }
    }

    if (direction.x < 0) {


        auto top_right_tile_x = (new_player_pos.x).floor();
        auto top_right_tile_y = (new_player_pos.y).floor();
        auto bottom_right_tile_x = (new_player_pos.x).floor();
        auto bottom_right_tile_y = (new_player_pos.y).ceil();

        if (is_wall(map, Pos(new_player_pos.x, new_player_pos.y.floor()))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_right_tile_x, top_right_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }

        if (is_wall(map, Pos(new_player_pos.x, new_player_pos.y.ceil()))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(bottom_right_tile_x, bottom_right_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }
    }

    if (direction.y > 0) {

        auto bottom_left_tile_x = (new_player_pos.x).floor();
        auto bottom_left_tile_y = (new_player_pos.y + 1).floor();
        auto bottom_right_tile_x = (new_player_pos.x).ceil();
        auto bottom_right_tile_y = (new_player_pos.y + 1).floor();

        if (is_wall(map, Pos(new_player_pos.x.floor(), new_player_pos.y + 1))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(bottom_left_tile_x, bottom_left_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }

        if (is_wall(map, Pos(new_player_pos.x.ceil(), new_player_pos.y + 1))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(bottom_right_tile_x, bottom_right_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }
    }

    if (direction.y < 0) {
        auto top_left_tile_x = (new_player_pos.x).floor();
        auto top_left_tile_y = (new_player_pos.y).floor();
        auto top_right_tile_x = (new_player_pos.x).ceil();
        auto top_right_tile_y = (new_player_pos.y).floor();

        if (is_wall(map, Pos(new_player_pos.x.floor(), new_player_pos.y))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_left_tile_x, top_left_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }

        if (is_wall(map, Pos(new_player_pos.x.ceil(), new_player_pos.y))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_right_tile_x, top_right_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }
    }

    return new_player_pos;
}

GameState GameState::update(Pos direction)
{
    static const int GHOST_TIMEOUT = 30;
    if (this->has_lost() || this->has_won()) {
        return {map, state, state_number + 1, ghosts, player.position, player_lives, exit, keys};
    }

    auto new_game_state = GameStatus::Active;


    // auto new_player_pos = this->player.position;
    auto new_player_pos = player_wall_check(player.position, direction, map);

    if (get_map_element(map, new_player_pos) == MapElement::End) {
        if (keys.empty()) { new_game_state = GameStatus::Won; }
    }

    // Ghost pathfinding goes here

    uint8_t new_player_lives = player_lives;
    auto new_ghosts = ghosts;
    for (auto &ghost : new_ghosts) {
        if (is_box_and_circle_intersecting(ghost.position, player.position)) {
            if (player_lives == 1){new_game_state = GameStatus::Lost;}
            else{
                --new_player_lives;
                new_player_pos = map->get_player_start_position();
            }
            break;
        }
        if (this->state_number % GHOST_TIMEOUT == 0) {
            auto new_position = ghost.get_next_pos(map, player.position);
            if (! is_wall(map, new_position)) { ghost.position = new_position; }
        }
    }

    auto new_keys = keys;
    for (int i = 0; i < new_keys.length();) {
        if (is_box_and_circle_intersecting(keys[i], player.position)) {
            new_keys.remove(i);
        } else {
            i++;
        }
    }

    return {map, new_game_state, state_number + 1, new_ghosts, new_player_pos, new_player_lives, exit, new_keys};
}

bool GameState::has_lost()
{
    return this->state == GameStatus::Lost;
}

bool GameState::has_won()
{
    return this->state == GameStatus::Won;
}

uint8_t GameState::get_life_count() {
    return player_lives;
}

}    // namespace game