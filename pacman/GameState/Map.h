#ifndef PACMAN_MAP_H
#define PACMAN_MAP_H

#include "Pos.h"

#include <QSharedData>
#include <QVector>
#include <cstddef>
#include <istream>
#include <optional>

namespace game
{

enum class MapElement {
    Wall,
    EmptySpace,
    Start,
    End,
};

class Map : public QSharedData
{
    std::size_t width;
    std::size_t height;
    QVector<QVector<MapElement>> map;

public:
    [[nodiscard]] std::size_t get_width() const;
    [[nodiscard]] std::size_t get_height() const;
    [[nodiscard]] const QVector<QVector<MapElement>> &get_map() const;
    [[nodiscard]] Pos get_player_start_position() const;

    Map(std::size_t width, std::size_t height, QVector<QVector<MapElement>> map);
};

}    // namespace game

#endif    // PACMAN_MAP_H
