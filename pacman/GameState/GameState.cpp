#include "GameState.h"

#include <utility>

// Helper constant signifying 1/2
const game::FixedPointNum<std::int64_t, 3> ONE_HALF = game::FixedPointNum<std::int64_t, 3>(1) / 2;

namespace game
{
// All boxes are 8 pixels or 1 tile high and wide
bool are_boxes_intersecting(Pos box_1, Pos box_2)
{
    bool is_x_intersecting = box_1.x > box_2.x && box_1.x < (box_2.x + 1);
    bool is_y_intersecting = box_1.y > box_2.y && box_1.y < (box_2.y + 1);

    return is_x_intersecting && is_y_intersecting;
}

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

MapElement get_map_element(const QExplicitlySharedDataPointer<Map> &map, Pos pos)
{
    if (pos.x < 0 || pos.x >= map->get_width()) { return MapElement::Wall; }

    if (pos.y < 0 || pos.y >= map->get_height()) { return MapElement::Wall; }

    return map->get_map()[pos.y.floor()][pos.x.floor()];
}

bool is_wall(const QExplicitlySharedDataPointer<Map> &map, Pos pos)
{
    return get_map_element(map, pos) == MapElement::Wall;
}


GameState::GameState(QExplicitlySharedDataPointer<Map> map, GameStatus status, uint64_t state_number,
                     QVector<Ghost> ghosts, Player player, Pos exit, QVector<Pos> keys)
    : map(std::move(map)), state(status), state_number(state_number), ghosts(std::move(ghosts)), player(player),
      exit(exit), keys(std::move(keys))
{
}

Pos player_wall_check(Pos player_pos, Pos direction, const QExplicitlySharedDataPointer<Map> &map)
{
    auto new_player_pos = player_pos + direction;

    // The player sprite is shifted one pixel to the left so, we manually shift it for collision checks;
    const auto one_pixel = FixedPointNum<std::int64_t, 3>(1) / 8;


    // Now do wall collisions
    if (direction.x > 0) {
        auto corrected_player_pos = new_player_pos;
        corrected_player_pos.x -= one_pixel;

        auto top_left_tile_x = (new_player_pos.x + 1).floor();
        auto top_left_tile_y = (new_player_pos.y).floor();
        auto bottom_left_tile_x = (new_player_pos.x + 1).floor();
        auto bottom_left_tile_y = (new_player_pos.y).ceil();

        if (is_wall(map, Pos(player_pos.x + 1, player_pos.y.floor()))) {
            if (is_box_and_circle_intersecting(corrected_player_pos, Pos(top_left_tile_x, top_left_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }

        if (is_wall(map, Pos(player_pos.x + 1, player_pos.y.ceil()))) {
            if (is_box_and_circle_intersecting(corrected_player_pos, Pos(bottom_left_tile_x, bottom_left_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }
    }

    if (direction.x < 0) {
        auto top_right_tile_x = (new_player_pos.x).floor();
        auto top_right_tile_y = (new_player_pos.y).floor();
        auto bottom_right_tile_x = (new_player_pos.x).floor();
        auto bottom_right_tile_y = (new_player_pos.y).ceil();

        if (is_wall(map, Pos(player_pos.x, player_pos.y.floor()))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_right_tile_x, top_right_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }

        if (is_wall(map, Pos(player_pos.x, player_pos.y.ceil()))) {
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

        if (is_wall(map, Pos(player_pos.x.floor(), player_pos.y + 1))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(bottom_left_tile_x, bottom_left_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }

        if (is_wall(map, Pos(player_pos.x.ceil(), player_pos.y + 1))) {
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

        if (is_wall(map, Pos(player_pos.x.floor(), player_pos.y))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_left_tile_x, top_left_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }

        if (is_wall(map, Pos(player_pos.x.ceil(), player_pos.y))) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_right_tile_x, top_right_tile_y))) {
                new_player_pos.y = player_pos.y;
            }
        }
    }

    return new_player_pos;
}
GameState GameState::update(Pos direction)
{
    if (this->has_lost() || this->has_won()) {
        return {map, state, state_number + 1, ghosts, player.position, exit, keys};
    }

    auto new_game_state = GameStatus::Active;


    // auto new_player_pos = this->player.position;
    auto new_player_pos = player_wall_check(player.position, direction, map);

    if (get_map_element(map, new_player_pos) == MapElement::End) {
        if (keys.empty()) { new_game_state = GameStatus::Won; }
    }

    // Ghost pathfinding goes here


    for (Ghost ghost : ghosts) {
        if (is_box_and_circle_intersecting(ghost.position, player.position)) {
            new_game_state = GameStatus::Lost;
            break;
        }
    }

    auto new_keys = keys;
    auto keys_to_remove = std::vector<int>();

    for (int i = 0; i < keys.length(); ++i) {
        if (is_box_and_circle_intersecting(keys[i], player.position)) { keys_to_remove.push_back(i); }
    }

    // If we delete an element it will shift all the following elements we need to account for that;
    int delete_shift = 0;
    if (! keys_to_remove.empty()) {
        for (int key_index : keys_to_remove) {
            new_keys.remove(key_index - delete_shift);
            delete_shift++;
        }
    }

    return {map, new_game_state, state_number + 1, ghosts, new_player_pos, exit, new_keys};
}
bool GameState::has_lost()
{
    return this->state == GameStatus::Lost;
}
bool GameState::has_won()
{
    return this->state == GameStatus::Won;
}
}    // namespace game