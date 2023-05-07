//
// Created by samuel on 7.5.2023.
//

#ifndef PACMAN_SCREENENDGAME_H
#define PACMAN_SCREENENDGAME_H
#include <QWidget>
#include <QLabel>

namespace view{
class ScreenEndGame : public QWidget {

Q_OBJECT
public:
    ScreenEndGame(QWidget *parent = nullptr);
public slots:
    void on_set_text(const QString& text);

private:
    QLabel *label{};
};
}


#endif //PACMAN_SCREENENDGAME_H
