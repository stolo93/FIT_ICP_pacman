#ifndef PACMAN_GAMESTATE_H
#define PACMAN_GAMESTATE_H

#include "Ghost.h"
#include "Map.h"
#include "Pos.h"

#include <QExplicitlySharedDataPointer>
#include <memory>

namespace game
{

struct GameState {
public:
    const uint64_t state_number;
    const QExplicitlySharedDataPointer<Map> map;
    const QVector<Pos> ghosts;
    const Pos player_pos;
    const Pos exit;
    const QVector<Pos> keys;


    GameState update();
};


}    // namespace game


#endif    // PACMAN_GAMESTATE_H
