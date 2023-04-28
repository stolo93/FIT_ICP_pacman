/**
 * @file item_wall.h
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_ITEMWALL_H
#define PACMAN_ITEMWALL_H

#include "./pacman_graphics_item.h"

namespace view
{
class GraphicsItemWall : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemWall(const QString &picture_path = "./Pictures/ghost.png");
};
}    // namespace view


#endif    // PACMAN_ITEMWALL_H
