/**
 * @file pacman_graphics_item.h
 * @author Samuel Stolarik
 * @date 2023-04-28
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_PACMANGRAPHICSITEM_H
#define PACMAN_PACMANGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <memory>

namespace view
{

class PacmanGraphicsItem : public QGraphicsItem
{
public:
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    std::unique_ptr<QPixmap> icon {};
    uint32_t width = 1;
    uint32_t height = 1;
};

}    // namespace view

#endif    // PACMAN_PACMANGRAPHICSITEM_H
