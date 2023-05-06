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

const uint8_t MAX_STATUS = 2;
enum class GameStatus : std::uint8_t {
    Active = 0,
    Lost = 1,
    Won = 2,
};

struct GameState {
public:
    const GameStatus state;
    const uint64_t state_number;
    const QExplicitlySharedDataPointer<Map> map;
    const QVector<Ghost> ghosts;
    const Player player;
    const Pos exit;
    const QVector<Pos> keys;


    GameState update(Pos direction);
    GameState(QExplicitlySharedDataPointer<Map> map, GameStatus status, uint64_t state_number, QVector<Ghost> ghosts,
              Player player, Pos exit, QVector<Pos> keys);

    bool has_won();
    bool has_lost();
};


}    // namespace game


#endif    // PACMAN_GAMESTATE_H
