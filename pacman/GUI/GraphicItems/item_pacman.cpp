/**
 * @file item_pacman.cpp
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#include "item_pacman.h"
namespace view
{
GraphicsItemPacman::GraphicsItemPacman(const QString &picture_path)
{
    this->icon = std::make_unique<QPixmap>(picture_path);
}
}    // namespace view