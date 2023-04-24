/**
 * @file screen_game.h
 * @author Samuel Stolarik
 * @date 2023-04-24
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_SCREENGAME_H
#define PACMAN_SCREENGAME_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class ScreenGame : public QWidget {
Q_OBJECT

public:
    explicit ScreenGame(QWidget *parent);

private slots:

    void on_set_scene(QGraphicsScene *scene);

private:
    QGraphicsView *game_view;
    QGraphicsScene *game_scene = nullptr;

};


#endif //PACMAN_SCREENGAME_H
