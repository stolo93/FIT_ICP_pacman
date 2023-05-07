/**
* @file parse.h
* @date 2023-04-27
* Project - ICP - project PacMan
 */

#ifndef PACMAN_PARSE_H
#define PACMAN_PARSE_H

#include "GameState.h"
#include "Map.h"

#include <optional>
#include <utility>
#include <variant>

namespace game
{

/**
 * Bytes used while serializing and deserializing maps to mark the start of different structures.
 */
enum class SerializationTags : std::uint8_t { StateTag = 0x1F, GhostTag = 0x2F, PlayerTag = 0x3F };

/**
 * Parses the map form a given input.
 * @param Stream to read the map data from.
 * @return The parsed map and the games state it's in.
 */
std::optional<std::pair<QExplicitlySharedDataPointer<Map>, GameState>> parse_map_from_stream(std::istream &input);

/**
 * Reads a game state from a stream.
 * @param input The stream to parse from.
 * @param map_to_attach Each state is attached to a map to save on bytes this map is contained in the log file only once. This is the map the state should be attached to.
 * @return The game state contained in the stream.
 */
std::optional<GameState> parse_state_from_stream(std::istream &input, QExplicitlySharedDataPointer<Map> map_to_attach);

/**
 * Serializes a game state into a file.
 * @param output The stream to write to.
 * @param state The state to serialize.
 * @return Returns an empty optional on write failure otherwise, the optional contains a monostate
 */
std::optional<std::monostate> write_state_to_stream(std::ostream &output, const GameState &state);

}    // namespace game

#endif    // PACMAN_PARSE_H
