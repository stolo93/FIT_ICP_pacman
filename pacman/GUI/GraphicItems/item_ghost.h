/**
 * @file item_ghost.h
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */


#ifndef PACMAN_ITEM_GHOST_H
#define PACMAN_ITEM_GHOST_H

#include "./pacman_graphics_item.h"

namespace view
{
class GraphicsItemGhost : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemGhost(const QString &picture_path = "./Pictures/ghost.png");
};
}    // namespace view


#endif    // PACMAN_ITEM_GHOST_H
