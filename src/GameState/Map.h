/**
* @file Map.h
* @author Adam Bezák (xbezak02)
* @date 2023-05-07
* Project - ICP - project PacMan
 */


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

/**
 * Type of tile
 */
enum class MapElement {
    Wall,
    EmptySpace,
    Start,
    End,
};

/**
 * Game map.
 */
class Map : public QSharedData
{
    /**
     * Map width.
     */
    std::size_t width;

    /**
     * Map height.
     */
    std::size_t height;

    /**
     * Vector of rows the map is composed of. Each row is composed of a series of tiles.
     */
    QVector<QVector<MapElement>> map;

public:
    /**
     * Gets the map width.
     * @return The width of the map.
     */
    [[nodiscard]] std::size_t get_width() const;

    /**
     * Gets the map height.
     * @return The height of the map.
     */
    [[nodiscard]] std::size_t get_height() const;

    /**
     * Returns the underlying map vector.
     * @return Const reference to map contents.
     */
    [[nodiscard]] const QVector<QVector<MapElement>> &get_map() const;

    /**
     * Gets the location the player should start at.
     * @return The location the player should start at.
     */
    [[nodiscard]] Pos get_player_start_position() const;

    /**
     * Map constructor
     * @param width
     * @param height
     * @param map
     */
    Map(std::size_t width, std::size_t height, QVector<QVector<MapElement>> map);
};

}    // namespace game

#endif    // PACMAN_MAP_H
