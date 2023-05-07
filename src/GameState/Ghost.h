#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H

#include "Pos.h"
#include "Map.h"
#include <QExplicitlySharedDataPointer>

namespace game
{
class Ghost
{
public:
    Pos position;

    Ghost(Pos position);

    Pos get_next_pos(const QExplicitlySharedDataPointer<Map>& map, const Pos& player_position);
};
}    // namespace game

#endif    // PACMAN_GHOST_H
