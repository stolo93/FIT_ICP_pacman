/**
 * @file menu_screen.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_MENUSCREEN_H
#define PACMAN_MENUSCREEN_H

#include <QWidget>

/**
 * @class ScreenMenu
 */
class ScreenMenu : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenMenu(QWidget *parent = nullptr);

signals:
    /**
     * Choose new game screen
     */
    void go_to_new_game_screen();

    /**
     * Choose load game screen
     */
    void go_to_load_game_screen();

private slots:
    /**
     * Handle new game button
     */
    void on_new_game_button();

    /**
     * Handle load game button
     */
    void on_load_game_button();
};


#endif    // PACMAN_MENUSCREEN_H
