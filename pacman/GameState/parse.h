#ifndef PACMAN_PARSE_H
#define PACMAN_PARSE_H

#include "GameState.h"
#include "Map.h"

#include <optional>
#include <utility>
namespace game
{

enum class SerializationTags : std::uint8_t { StateTag = 0x01, GhostTag = 0x02, PlayerTag = 0x3 };

std::optional<std::pair<QExplicitlySharedDataPointer<Map>, GameState>> parse_map_from_stream(std::istream &input);
bool write_map_to_stream(std::ostream &output);
std::optional<GameState> parse_state_from_stream(std::istream &input, QExplicitlySharedDataPointer<Map> map_to_attach);
bool write_state_to_stream(std::ostream &output);
}    // namespace game

#endif    // PACMAN_PARSE_H
