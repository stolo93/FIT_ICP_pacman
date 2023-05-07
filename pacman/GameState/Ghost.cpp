#include "Ghost.h"
#include "astar.h"

namespace game
{
Ghost::Ghost(Pos position) : position(position) {}

    Pos Ghost::get_next_pos(const QExplicitlySharedDataPointer<Map> &map, const Pos &player_position) {
        auto path = A_Star::dumb_move(map, this->position, player_position);
        if (path.empty()){
            return {0,0};
        }

        return path[1];
    }
}    // namespace game