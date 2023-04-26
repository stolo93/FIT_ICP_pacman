#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H

#include "Pos.h"
namespace game
{
class Ghost
{
public:
    Pos position;

    Ghost(Pos position);
};
}    // namespace game

#endif    // PACMAN_GHOST_H
