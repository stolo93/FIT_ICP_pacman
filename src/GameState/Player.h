#ifndef PACMAN_PLAYER_H
#define PACMAN_PLAYER_H

#include "Pos.h"
namespace game
{

class Player
{

public:
    Pos position;

    Player(Pos position);
};

}    // namespace game

#endif    // PACMAN_PLAYER_H
