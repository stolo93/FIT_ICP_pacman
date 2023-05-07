/**
* @file Player.h
* @date 2023-04-26
* Project - ICP - project PacMan
 */

#ifndef PACMAN_PLAYER_H
#define PACMAN_PLAYER_H

#include "Pos.h"
namespace game
{

/**
 * A player.
 *
 * "Hyah!"
 *      - Link (Super Mario Smash Bros. Melee)
 */
class Player
{

public:
    /**
     * The Player location.
     */
    Pos position;

    /**
     * Player constructor.
     * @param position
     */
    Player(Pos position);
};

}    // namespace game

#endif    // PACMAN_PLAYER_H
