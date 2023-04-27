/**
 * @file game_view.h
 * @author Samuel Stolarik
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_GAMEVIEW_H
#define PACMAN_GAMEVIEW_H

#include "../GameState/GameState.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <memory>

namespace view {
    class GameView : public QGraphicsView {

    Q_OBJECT

    public:
        explicit GameView(QGraphicsView *parent = nullptr);

    signals:

        void user_event(QKeyEvent *event);


    public slots:

        void on_init_scene(std::shared_ptr<game::GameState> game_state);

        void on_update_scene(std::shared_ptr<game::GameState> game_state);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;

    private:
        std::unique_ptr<QGraphicsScene> scene;
        
    };
}


#endif //PACMAN_GAMEVIEW_H
