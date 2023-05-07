/**
 * @file pacman_graphics_item.cpp
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#include "pacman_graphics_item.h"

namespace view
{
QRectF PacmanGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, this->width, this->height);
}

void PacmanGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(this->boundingRect(), *icon, icon->rect());

}
}    // namespace view
