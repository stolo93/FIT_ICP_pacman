#include "parse.h"

#include "boost/endian/buffers.hpp"

std::optional<std::size_t> parse_number_from_stream(std::istream &input)
{
    auto text_value = std::string();

    char character;
    while (input.get(character)) {
        if (character == std::iswspace(character)) { break; }

        if (! std::isdigit(character)) { return {}; }

        text_value += character;
    }

    if (text_value.empty()) { return {}; }

    std::size_t value;
    // use sscanf instead of strtoul because size_t may not bet a unsigned long on some platforms
    if (sscanf(text_value.c_str(), "%zu", &value) != 1) { return {}; }

    return value;
}

namespace game
{
std::optional<std::pair<QExplicitlySharedDataPointer<Map>, GameState>> parse_from_stream(std::istream &input)
{
    auto possibly_height = parse_number_from_stream(input);
    if (! possibly_height) { return {}; }
    auto height = possibly_height.value();

    if (! input) { return {}; }

    auto possibly_width = parse_number_from_stream(input);
    if (! possibly_width) { return {}; }
    auto width = possibly_width.value();

    if (! input) { return {}; }

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
                    row_contents.push_back(MapElement::EmptySpace);
                    break;
                case 'T':
                    if (maybe_exit) {
                        // Redefining exit position
                        return {};
                    }

                    maybe_exit = Pos(column, row);
                    row_contents.push_back(MapElement::EmptySpace);
                    break;
                default:
                    // Unknown map element
                    return {};
            }
        }

        // Every line of the map definition should end with a newline
        char character;
        if (! input.get(character)) { return {}; }
        if (character != '\n') { return {}; }

        map.push_back(row_contents);
    }

    if (! maybe_player || ! maybe_exit) {
        // No exit or no player were defined
        return {};
    }

    auto map_ptr = QExplicitlySharedDataPointer<Map>(new Map(width, height, map));
    auto first_state = GameState(map_ptr, 0, ghosts, maybe_player.value(), maybe_exit.value(), keys);

    return std::pair {map_ptr, first_state};
}

std::optional<GameState> parse_state_from_stream(std::istream &input)
{
    /*
     * The field layout for a saved GameState is:
     *
     * StateTag, 1 byte (constant, always set to 0x01)
     * state_number, 8 byte little endian integer
     * exit, sizeof(Pos) bytes
     * player, sizeof(Player) bytes
     * keys, dynamically sized vector of Pos structs
     * ghosts, dynamically sized vector of Ghost structs
     *
     * Vectors are laid out as size 8 byte little endian integer specifying the number of elements that follow
     * elements n*size bytes of elements inside the vector
     *
     * The pos struct is serialized as 2 8 byte little endian integers
     */

    // The tags have to be loaded
    std::uint8_t state_tag;
    input >> state_tag;
    if (! input) {
        // input can't provide data while we still need it
        return {};
    }

    if (state_tag != static_cast<std::uint8_t>(SerializationTags::StateTag)) {
        // Whatever we just tried to load isn't the GameState struct
        return {};
    }

    boost::endian::little_int64_buf_t pure_data[3] = {};
    auto x = pure_data[0].value();

    return {};
}

}    // namespace game