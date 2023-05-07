/**
 * @file livesdisplayimage.h
 * @author Samuel Stolarik
 * @date 2023-05-05
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_LIVESDISPLAYIMAGE_H
#define PACMAN_LIVESDISPLAYIMAGE_H
#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

namespace view
{

/**
 * @class LivesDisplayImage
 * @brief Graphic live counter with heart emojis
 */
class LivesDisplayImage : public QWidget
{
    Q_OBJECT

public:
    explicit LivesDisplayImage(const QString &pixmap_name = ":img/resources/game_objects/heart.png", int count = 1,
                               QWidget *parent = nullptr);

    /**
     * Update lives count
     * @param count
     */
    void update_lives_count(int count);

protected:
    /**
     * Draw lives
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * Heart emoji
     */
    QPixmap m_pixmap;

    /**
     * Live count
     */
    int m_count;
};


}    // namespace view

#endif    // PACMAN_LIVESDISPLAYIMAGE_H
