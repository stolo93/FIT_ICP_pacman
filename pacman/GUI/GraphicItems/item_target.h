/**
 * @file item_target.h
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_ITEM_TARGET_H
#define PACMAN_ITEM_TARGET_H

#include "pacman_graphics_item.h"

namespace view
{

class GraphicsItemTarget : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemTarget(const QString &picture_path = "./Pictures/target.png");
};

}    // namespace view

#endif    // PACMAN_ITEM_TARGET_H
