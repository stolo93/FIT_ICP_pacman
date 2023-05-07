/**
* @file parse.cpp
* @date 2023-04-27
* Project - ICP - project PacMan
*/

#include "parse.h"

#include "boost/endian/buffers.hpp"

#include <utility>
#include <variant>

#define READ_OR_RETURN_EMPTY(stream, variable)                                                                         \
    (stream).read((char *) &variable, sizeof(variable));                                                               \
    if (! stream) { return {}; }

#define READ_BUF_OR_RETURN_EMPTY(stream, buf)                                                                          \
    (stream).read((char *) (buf).data(), sizeof(buf));                                                                 \
    if (! stream) { return {}; }

#define WRITE_OR_RETURN_EMPTY(stream, value)                                                                           \
    (stream).write((const char *) &value, sizeof(value));                                                              \
    if (! stream) { return {}; }

#define WRITE_BUF_OR_RETURN_EMPTY(stream, buf)                                                                         \
    (stream).write((const char *) (buf).data(), sizeof(buf));                                                          \
    if (! stream) { return {}; }

/**
 * Parses a decimal from a stream.
 * @param input the stream to parse from.
 * @return An optional containing the parsed number if successful, an empty one otherwise.
 */
std::optional<std::size_t> parse_number_from_stream(std::istream &input)
{
    auto text_value = std::string();

    char character;
    while (input.get(character)) {
        if (iswspace(character)) { break; }

        if (! std::isdigit(character)) { return {}; }

        text_value += character;
    }

    if (text_value.empty()) { return {}; }

    std::size_t value;
    // use sscanf instead of strtoul because size_t may not bet a unsigned long on some platforms
    if (sscanf(text_value.c_str(), "%zu", &value) != 1) { return {}; }

    return value;
}

enum class LineEnd { EndOfFile, NewLine, Character };

/**
 * Attempts to consume the end of line will manage crcf line endings along with unix line endings.
 * @param input The stream to read the line ending from.
 * @return The result of the parsing operation.
 */
LineEnd parse_line_end(std::istream &input)
{
    char character;
    if (! input.get(character)) { return LineEnd::EndOfFile; }

    if (character == '\n') { return LineEnd::NewLine; }

    if (character != '\r') { return LineEnd::Character; }

    if (! input.get(character)) { return LineEnd::EndOfFile; }

    if (character != '\n') { return LineEnd::Character; }

    return LineEnd::NewLine;
}

namespace game
{
std::optional<std::pair<QExplicitlySharedDataPointer<Map>, GameState>> parse_map_from_stream(std::istream &input)
{
    auto possibly_height = parse_number_from_stream(input);
    if (! possibly_height) { return {}; }
    auto height = possibly_height.value();

    if (! input) { return {}; }

    auto possibly_width = parse_number_from_stream(input);
    if (! possibly_width) { return {}; }
    auto width = possibly_width.value();

    if (! input) { return {}; }
    if (input.peek() == EOF) { return {}; }
    if (input.peek() == '\n') { input.get(); }

    std::optional<Player> maybe_player = {};
    QVector<Ghost> ghosts = {};
    QVector<Pos> keys = {};
    std::optional<Pos> maybe_exit = {};

    QVector<QVector<MapElement>> map = {};

    for (std::size_t row = 0; row < height; ++row) {
        QVector<MapElement> row_contents = {};

        for (std::size_t column = 0; column < width; ++column) {
            char map_element;
            if (! input.get(map_element)) { return {}; }

            switch (map_element) {
                case '.':
                    row_contents.push_back(MapElement::EmptySpace);
                    break;
                case 'X':
                    row_contents.push_back(MapElement::Wall);
                    break;
                case 'G': {
                    auto ghost = Ghost(Pos(column, row));
                    ghosts.push_back(ghost);
                    row_contents.push_back(MapElement::EmptySpace);
                    break;
                }
                case 'K':
                    keys.push_back(Pos(column, row));
                    row_contents.push_back(MapElement::EmptySpace);
                    break;
                case 'S':
                    if (maybe_player) {
                        // Redefining player position
                        return {};
                    }

                    maybe_player = Player(Pos(column, row));
                    row_contents.push_back(MapElement::Start);
                    break;
                case 'T':
                    if (maybe_exit) {
                        // Redefining exit position
                        return {};
                    }

                    maybe_exit = Pos(column, row);
                    row_contents.push_back(MapElement::End);
                    break;
                default:
                    // Unknown map element
                    return {};
            }
        }

        // Every line of the map definition should end with a newline or crcf
        auto end_line = parse_line_end(input);
        if (end_line == LineEnd::Character) { return {}; }

        // The last line might end with an EOF
        if (row != height - 1 && end_line == LineEnd::EndOfFile) { return {}; }

        map.push_back(row_contents);
    }

    if (! maybe_player || ! maybe_exit) {
        // No exit or no player were defined
        return {};
    }

    auto map_ptr = QExplicitlySharedDataPointer<Map>(new Map(width, height, map));
    uint8_t player_lives = 3;
    auto first_state = GameState(map_ptr, GameStatus::Active, 0, ghosts, maybe_player.value(), player_lives,
                                 maybe_exit.value(), keys);

    return std::pair {map_ptr, first_state};
}

std::optional<Pos> parse_pos_from_stream(std::istream &input)
{
    /*
     * The pos struct is serialized as:
     * x, 8 byte little endian integer
     * y, 8 byte little endian integer
     */

    boost::endian::little_int64_buf_t x, y;
    READ_BUF_OR_RETURN_EMPTY(input, x)
    READ_BUF_OR_RETURN_EMPTY(input, y)

    // Fixed point integers are saved byte for byte
    return Pos(FixedPointNum<std::int64_t, 3>::from_bits(x.value()),
               FixedPointNum<std::int64_t, 3>::from_bits(y.value()));
}

std::optional<Player> parse_player_from_stream(std::istream &input)
{
    /*
     * The Player class is serialized as:
     *
     * tag, 1 byte tag that's always SerializationTags::PlayerTag
     * position, sizeof(Pos) Pos struct
     */

    std::uint8_t tag;
    READ_OR_RETURN_EMPTY(input, tag)
    if (! input) { return {}; }

    if (tag != static_cast<std::uint8_t>(SerializationTags::PlayerTag)) { return {}; }

    std::optional<Pos> maybe_pos = parse_pos_from_stream(input);
    if (! maybe_pos) { return {}; }

    return Player(maybe_pos.value());
}

std::optional<Ghost> parse_ghost_from_stream(std::istream &input)
{
    /*
     * The Ghost class is serialized as:
     *
     * tag, 1 byte tag that's always SerializationTags::GhostTag
     * position, sizeof(Pos) Pos struct
     */

    std::uint8_t tag;
    READ_OR_RETURN_EMPTY(input, tag)
    if (! input) { return {}; }

    if (tag != static_cast<std::uint8_t>(SerializationTags::GhostTag)) { return {}; }

    std::optional<Pos> maybe_pos = parse_pos_from_stream(input);
    if (! maybe_pos) { return {}; }

    return Ghost(maybe_pos.value());
}

std::optional<GameState> parse_state_from_stream(std::istream &input, QExplicitlySharedDataPointer<Map> map_to_attach)
{
    /*
     * The field layout for a saved GameState is:
     *
     * StateTag, 1 unsigned byte (constant, always set to SerializationTags::StateTag)
     * status, 1 unsigned byte
     * live count, 1 unsigned byte
     * state_number, 8 byte little endian integer
     * exit, sizeof(Pos) bytes
     * player, sizeof(Player) bytes
     * keys, dynamically sized vector of Pos structs
     * ghosts, dynamically sized vector of Ghost structs
     *
     * Vectors are laid out as 8 bytes unsigned little endian integer specifying the length of the vector and
     * len * sizeof(element) bytes for the packed elements found in the vector
     *
     * The pos struct is serialized as 2 8 byte little endian integers
     */

    // The tags have to be loaded
    std::uint8_t state_tag;
    READ_OR_RETURN_EMPTY(input, state_tag)
    if (! input) {
        // input can't provide data while we still need it
        return {};
    }

    if (state_tag != static_cast<std::uint8_t>(SerializationTags::StateTag)) {
        // Whatever we just tried to load isn't the GameState struct
        return {};
    }

    std::uint8_t status;
    READ_OR_RETURN_EMPTY(input, status)
    if (status > MAX_STATUS) { return {}; }

    std::uint8_t lives;
    READ_OR_RETURN_EMPTY(input, lives);

    boost::endian::little_int64_buf_t state_number;
    READ_BUF_OR_RETURN_EMPTY(input, state_number)

    std::optional<Pos> maybe_exit_location = parse_pos_from_stream(input);
    if (! maybe_exit_location) { return {}; }

    std::optional<Player> maybe_player = parse_player_from_stream(input);

    boost::endian::little_uint64_buf_t key_count;
    READ_BUF_OR_RETURN_EMPTY(input, key_count)
    auto keys = QVector<Pos>();

    for (uint64_t i = 0; i < key_count.value(); i++) {
        std::optional<Pos> maybe_key = parse_pos_from_stream(input);
        if (! maybe_key) { return {}; }
        keys.push_back(maybe_key.value());
    }

    boost::endian::little_uint64_buf_t ghost_num;
    READ_BUF_OR_RETURN_EMPTY(input, ghost_num)
    auto ghosts = QVector<Ghost>();

    for (uint64_t i = 0; i < ghost_num.value(); i++) {
        std::optional<Ghost> maybe_ghost = parse_ghost_from_stream(input);
        if (! maybe_ghost) { return {}; }
        ghosts.push_back(maybe_ghost.value());
    }
    return GameState(std::move(map_to_attach), GameStatus(status), state_number.value(), ghosts, maybe_player.value(),
                     lives, maybe_exit_location.value(), keys);
}

std::optional<std::monostate> write_pos_to_stream(std::ostream &output, const Pos &pos)
{
    auto x = boost::endian::little_int64_buf_t(pos.x.to_bits());
    auto y = boost::endian::little_int64_buf_t(pos.y.to_bits());

    WRITE_BUF_OR_RETURN_EMPTY(output, x)
    WRITE_BUF_OR_RETURN_EMPTY(output, y)

    return std::monostate();
}

std::optional<std::monostate> write_player_to_stream(std::ostream &output, const Player &player)
{
    auto tag = static_cast<std::uint8_t>(SerializationTags::PlayerTag);
    WRITE_OR_RETURN_EMPTY(output, tag)
    if (! write_pos_to_stream(output, player.position)) { return {}; }

    return std::monostate();
}

std::optional<std::monostate> write_ghost_to_stream(std::ostream &output, const Ghost &ghost)
{
    auto tag = static_cast<std::uint8_t>(SerializationTags::GhostTag);
    WRITE_OR_RETURN_EMPTY(output, tag)
    if (! write_pos_to_stream(output, ghost.position)) { return {}; }

    return std::monostate();
}

// Returns empty optional on failure to write and an optional containing std::monostate on success
std::optional<std::monostate> write_state_to_stream(std::ostream &output, const GameState &state)
{
    auto tag = static_cast<std::uint8_t>(SerializationTags::StateTag);
    WRITE_OR_RETURN_EMPTY(output, tag)

    auto cast_state = static_cast<std::uint8_t>(state.state);
    WRITE_OR_RETURN_EMPTY(output, cast_state)

    WRITE_OR_RETURN_EMPTY(output, state.player_lives)

    auto state_num = boost::endian::little_uint64_buf_t(state.state_number);
    WRITE_BUF_OR_RETURN_EMPTY(output, state_num)

    if (! write_pos_to_stream(output, state.exit)) { return {}; }

    if (! write_player_to_stream(output, state.player)) { return {}; }


    WRITE_BUF_OR_RETURN_EMPTY(output, boost::endian::little_uint64_buf_t(state.keys.length()))
    for (auto key : state.keys) {
        if (! write_pos_to_stream(output, key)) { return {}; }
    }

    WRITE_BUF_OR_RETURN_EMPTY(output, boost::endian::little_uint64_buf_t(state.ghosts.length()))
    for (auto ghost : state.ghosts) {
        if (! write_ghost_to_stream(output, ghost)) { return {}; }
    }

    return std::monostate();
}
}    // namespace game