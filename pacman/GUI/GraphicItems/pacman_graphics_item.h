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

// TODO change picture paths
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


class GraphicsItemPacman : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemPacman(const QString &picture_path = "/home/samuel/fit/4_semester/icp/FIT_ICP_pacman/pacman/GUI/GraphicItems/Pictures/pacman.png");
};


class GraphicsItemGhost : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemGhost(const QString &picture_path = "/home/samuel/fit/4_semester/icp/FIT_ICP_pacman/pacman/GUI/GraphicItems/Pictures/ghost.png");
};


class GraphicsItemWall : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemWall(const QString &picture_path = "/home/samuel/fit/4_semester/icp/FIT_ICP_pacman/pacman/GUI/GraphicItems/Pictures/wall.png");
};


class GraphicsItemTarget : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemTarget(const QString &picture_path = "/home/samuel/fit/4_semester/icp/FIT_ICP_pacman/pacman/GUI/GraphicItems/Pictures/target.png");
};


class GraphicsItemKey : public PacmanGraphicsItem
{
public:
    explicit GraphicsItemKey(const QString &picture_path = "/home/samuel/fit/4_semester/icp/FIT_ICP_pacman/pacman/GUI/GraphicItems/Pictures/key.png");
};


}    // namespace view

#endif    // PACMAN_PACMANGRAPHICSITEM_H
