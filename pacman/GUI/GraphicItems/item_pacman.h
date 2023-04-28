/**
 * @file item_pacman.h
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_ITEMPACMAN_H
#define PACMAN_ITEMPACMAN_H

#include "./pacman_graphics_item.h"

namespace view
{
class GraphicsItemPacman : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemPacman(const QString &picture_path = "./Pictures/pacman.png");
};
}    // namespace view


#endif    // PACMAN_ITEMPACMAN_H
