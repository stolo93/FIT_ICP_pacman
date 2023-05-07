/**
 * @file main_menu.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_WIDGET_MAIN_MENU_H
#define PACMAN_WIDGET_MAIN_MENU_H

#include <QWidget>

/**
 * @class MainMenu
 * @brief Main menu class
 */
class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    void clicked_new_game();
    void clicked_load_game();

private slots:
    void on_new_game_button();
    void on_load_game_button();
};


#endif    // PACMAN_WIDGET_MAIN_MENU_H
