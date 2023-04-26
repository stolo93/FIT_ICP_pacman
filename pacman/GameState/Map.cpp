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

}    // namespace game