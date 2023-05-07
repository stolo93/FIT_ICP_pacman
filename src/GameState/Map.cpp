/**
* @file Map.cpp
* @author Adam Bezák (xbezak02)
* @date 2023-05-07
* Project - ICP - project PacMan
 */

#include "Map.h"

#include <utility>

namespace game
{

std::size_t Map::get_width() const
{
    return this->width;
}

std::size_t Map::get_height() const
{
    return this->height;
}

const QVector<QVector<MapElement>> &Map::get_map() const
{
    return this->map;
}

Map::Map(std::size_t width, std::size_t height, QVector<QVector<MapElement>> map)
    : width(width), height(height), map(std::move(map))
{
}

    Pos Map::get_player_start_position() const {
        for (int i = 0; i < width; ++i){
            for (int j = 0; j < height; ++j){
                if (get_map()[j][i] == MapElement::Start){
                    return {i, j};
                }
            }
        }
        return {0,0};
    }

}    // namespace game