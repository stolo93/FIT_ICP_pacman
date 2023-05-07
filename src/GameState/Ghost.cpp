/**
* @file Ghost.cpp
* @author Adam Bezák (xbezak02)
* @date 2023-05-07
* Project - ICP - project PacMan
*/


#include "Ghost.h"

#include "astar.h"

namespace game
{
Ghost::Ghost(Pos position) : position(position) {}

Pos Ghost::get_next_pos(const QExplicitlySharedDataPointer<Map> &map, const Pos &player_position)
{
    // Very borked
    // auto path = A_Star::a_star(map, this->position, player_position);

    auto path = A_Star::dumb_move(map, this->position, player_position);
    if (path.empty()) { return {0, 0}; }

    return path[1];
}
}    // namespace game