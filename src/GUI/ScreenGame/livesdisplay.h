//
// Created by samuel on 7.5.2023.
//

#ifndef PACMAN_LIVESDISPLAY_H
#define PACMAN_LIVESDISPLAY_H
#include <QWidget>
#include <QLabel>

namespace view {

    class LivesDisplayer : public QWidget {
    Q_OBJECT
    public:
        LivesDisplayer(QWidget* parent = nullptr, uint8_t lives = 0);

        void update_lives_count(uint8_t lives);

    private:
        uint8_t lives_count;
        QLabel *lives_label;
    };

} // view

#endif //PACMAN_LIVESDISPLAY_H
