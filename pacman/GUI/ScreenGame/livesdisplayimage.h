//
// Created by samuel on 7.5.2023.
//

#ifndef PACMAN_LIVESDISPLAYIMAGE_H
#define PACMAN_LIVESDISPLAYIMAGE_H
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPixmap>

namespace view {

    class LivesDisplayImage : public QWidget {
    Q_OBJECT

    public:
        explicit LivesDisplayImage(const QString &pixmap_name = ":img/resources/game_objects/heart.png", int count = 1, QWidget *parent = nullptr);

        void update_lives_count(int count);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        QPixmap m_pixmap;
        int m_count;
    };


} // view

#endif //PACMAN_LIVESDISPLAYIMAGE_H
