/**
* @file Ghost.h
* @author Adam Bezák (xbezak02)
* @date 2023-05-07
* Project - ICP - project PacMan
 */

#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H

#include "Pos.h"
#include "Map.h"
#include <QExplicitlySharedDataPointer>

namespace game
{
/**
 * A ghost.
 * Boo!
 */
class Ghost
{
public:
    /**
     * The location of the ghost.
     */
    Pos position;

    /**
     * Creates a new ghost at the given position.
     * @param position
     */
    Ghost(Pos position);

    /**
     * Attempts to find a path from current location the target.
     * @param map The map we're pathfinding on.
     * @param player_position The position of the target.
     * @return the next position we should go to.
     */
    Pos get_next_pos(const QExplicitlySharedDataPointer<Map>& map, const Pos& player_position);
};
}    // namespace game

#endif    // PACMAN_GHOST_H
