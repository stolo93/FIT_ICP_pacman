#ifndef PACMAN_GAMESTATE_H
#define PACMAN_GAMESTATE_H

#include "Ghost.h"
#include "Map.h"
#include "Player.h"
#include "Pos.h"

#include <QExplicitlySharedDataPointer>
#include <memory>

namespace game
{

struct GameState {
public:
    const uint64_t state_number;
    const QExplicitlySharedDataPointer<Map> map;
    const QVector<Ghost> ghosts;
    const Player player;
    const Pos exit;
    const QVector<Pos> keys;


    GameState update(Pos direction);
    GameState(QExplicitlySharedDataPointer<Map> map, uint64_t state_number, QVector<Ghost> ghosts, Player player,
              Pos exit, QVector<Pos> keys);
};


}    // namespace game


#endif    // PACMAN_GAMESTATE_H
