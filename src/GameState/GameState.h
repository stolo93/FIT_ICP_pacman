/**
* @file GameState.h
* @author Adam Bezák (xbezak02)
* @date 2023-05-07
* Project - ICP - project PacMan
*/


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

/**
 * Maximum value of a GameStatus.
 */
const uint8_t MAX_STATUS = 2;

/**
 * A value signifying what state the game is in.
 */
enum class GameStatus : std::uint8_t {
    Active = 0,
    Lost = 1,
    Won = 2,
};

/**
 * A class holding the state of the game at a given point in time.
 */
class GameState {
public:
    const GameStatus state;
    const uint64_t state_number;
    const QExplicitlySharedDataPointer<Map> map;
    const QVector<Ghost> ghosts;
    const Player player;
    const uint8_t player_lives;
    const Pos exit;
    const QVector<Pos> keys;

    /**
     * Creates the next game state after applying player movement.
     * @param direction The direction the player want's to move.
     * @return The updated game state.
     */
    GameState update(Pos direction);

    /**
     * GameState constructor.
     * @param map
     * @param status
     * @param state_number
     * @param ghosts
     * @param player
     * @param player_lives
     * @param exit
     * @param keys
     */
    GameState(QExplicitlySharedDataPointer<Map> map, GameStatus status, uint64_t state_number, QVector<Ghost> ghosts,
              Player player,uint8_t player_lives, Pos exit, QVector<Pos> keys);

    /**
     * Checks if the current game state has reached the win condition.
     * @return true if the game has been won
     */
    bool has_won();

    /**
     * Checks if the current game state has reached the loss condition.
     * @return true if the game has been lost.
     */
    bool has_lost();

    /**
     * Gets the amount of lives the player still has left.
     */
    uint8_t get_life_count();
};


}    // namespace game


#endif    // PACMAN_GAMESTATE_H
