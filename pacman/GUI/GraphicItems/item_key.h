/**
 * @file item_key.cpp
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_ITEM_KEY_H
#define PACMAN_ITEM_KEY_H

#include "./pacman_graphics_item.h"

namespace view
{
class GraphicsItemKey : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemKey(const QString &picture_path = "./Pictures/key.png");
};
}    // namespace view


#endif    // PACMAN_ITEM_KEY_H
