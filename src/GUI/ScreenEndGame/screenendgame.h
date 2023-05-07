/**
 * @file screenendgame.h
 * @author Samuel Stolarik
 * @date 2023-05-06
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_SCREENENDGAME_H
#define PACMAN_SCREENENDGAME_H
#include <QLabel>
#include <QWidget>

namespace view
{
/**
 * @class ScreenEndGame
 */
class ScreenEndGame : public QWidget
{

    Q_OBJECT
public:
    ScreenEndGame(QWidget *parent = nullptr);
public slots:
    /**
     * Display text
     * @param text
     */
    void on_set_text(const QString &text);

private:
    /**
     * Text label
     */
    QLabel *label {};
};
}    // namespace view


#endif    // PACMAN_SCREENENDGAME_H
