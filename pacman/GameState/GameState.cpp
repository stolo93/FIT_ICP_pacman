#include "GameState.h"

#include <utility>

// Helper constant signifying 1/2
const game::FixedPointNum<std::int64_t, 3> ONE_HALF = game::FixedPointNum<std::int64_t, 3>(1) / 2;

// All boxes are 8 pixels or 1 tile high and wide
bool are_boxes_intersecting(game::Pos box_1, game::Pos box_2)
{
    bool is_x_intersecting = box_1.x > box_2.x && box_1.x < (box_2.x + 1);
    bool is_y_intersecting = box_1.y > box_2.y && box_1.y < (box_2.y + 1);

    return is_x_intersecting && is_y_intersecting;
}

bool is_box_and_circle_intersecting(game::Pos box, game::Pos circle)
{
    // https://stackoverflow.com/a/402010
    auto box_center = box + game::Pos(ONE_HALF, ONE_HALF);
    auto circle_center = circle + game::Pos(ONE_HALF, ONE_HALF);
    auto collision_vector = game::Pos((circle_center.x - box_center.x).abs(), (circle_center.y - box_center.y).abs());

    if (collision_vector.x > 1 || collision_vector.y > 1) { return false; }
    if (collision_vector.x <= ONE_HALF || collision_vector.y <= ONE_HALF) { return true; }

    auto corner_distance_squared = (collision_vector.x - ONE_HALF) * (collision_vector.x - ONE_HALF) +
                                   (collision_vector.y - ONE_HALF) * (collision_vector.y - ONE_HALF);

    return corner_distance_squared < 0;
}

namespace game
{
GameState::GameState(QExplicitlySharedDataPointer<Map> map, uint64_t state_number, QVector<Ghost> ghosts, Player player,
                     Pos exit, QVector<Pos> keys)
    : map(std::move(map)), state_number(state_number), ghosts(std::move(ghosts)), player(player), exit(exit),
      keys(std::move(keys))
{
}

Pos player_wall_check(Pos player_pos, Pos direction, const QExplicitlySharedDataPointer<Map> &map)
{
    auto new_player_pos = player_pos + direction;
    bool hit_left_wall = false;
    bool hit_right_wall = false;
    bool hit_top = false;
    bool hit_bottom = false;

    // Check if the player just moved into the edge of the world if so push him back
    if (new_player_pos.x < 0) {
        new_player_pos.x = player_pos.x;
        hit_left_wall = true;
    } else if (new_player_pos.x + 1 > map->get_width()) {
        new_player_pos.x = player_pos.x;
        hit_right_wall = true;
    }

    if (new_player_pos.y < 0) {
        new_player_pos.y = player_pos.y;
        hit_top = true;
    } else if (new_player_pos.y + 1 > map->get_height()) {
        new_player_pos.y = player_pos.y;
        hit_bottom = true;
    }

    // Now do wall collisions
    if (direction.x > 0 && ! hit_left_wall) {
        auto top_left_tile_x = (new_player_pos.x + 1).floor();
        auto top_left_tile_y = (new_player_pos.y).floor();
        auto top_right_tile_x = (new_player_pos.x + 1).floor();
        auto top_right_tile_y = (new_player_pos.y).ceil();
        auto tiles = map->get_map();

        if (tiles[top_left_tile_y][top_left_tile_x] == MapElement::Wall) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_left_tile_x, top_left_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }

        if (tiles[top_right_tile_x][top_right_tile_y] == MapElement::Wall) {
            if (is_box_and_circle_intersecting(new_player_pos, Pos(top_right_tile_x, top_right_tile_y))) {
                new_player_pos.x = player_pos.x;
            }
        }
    }

    return new_player_pos;
}
GameState GameState::update(Pos direction)
{
    // auto new_player_pos = this->player.position;
    auto new_player_pos = player_wall_check(this->player.position, direction, this->map);


    return {this->map, this->state_number + 1, this->ghosts, new_player_pos, this->exit, this->keys};
}
}    // namespace game