/**
 * @file livesdisplayimage.cpp
 * @author Samuel Stolarik
 * @date 2023-05-05
 * Project - ICP - project PacMan
 */

#include "livesdisplayimage.h"

namespace view
{
LivesDisplayImage::LivesDisplayImage(const QString &pixmap_name, int count, QWidget *parent)
    : QWidget(parent), m_pixmap(QPixmap(pixmap_name)), m_count(count)
{
}

void LivesDisplayImage::update_lives_count(int count)
{
    m_count = count;
    update();
}

void LivesDisplayImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int imageWidth = 24;
    int imageHeight = 24;
    int totalWidth = imageWidth * m_count;
    int startX = (width() - totalWidth) / 2;

    for (int i = 0; i < m_count; ++i) {
        QRect targetRect(startX + i * imageWidth, (height() - imageHeight) / 2, imageWidth, imageHeight);
        painter.drawPixmap(targetRect, m_pixmap.scaled(imageWidth, imageHeight, Qt::IgnoreAspectRatio));
    }
}
}    // namespace view