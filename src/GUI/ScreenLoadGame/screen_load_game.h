/**
 * @file scree_load_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */


#ifndef PACMAN_SCREENLOADGAME_H
#define PACMAN_SCREENLOADGAME_H

#include <QWidget>

/**
 * @class ScreenLoadGame
 */
class ScreenLoadGame : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenLoadGame(QWidget *parent = nullptr);

signals:

    /**
     * Go back button clicked
     */
    void go_to_home();

    /**
     * Start game button clicked
     * @param file_name_map
     */
    void replay_game(std::string &file_name_map);

private slots:

    /**
     * Handle go back button
     */
    void on_go_back_button();

    /**
     * Handle file entered
     * @param file_name
     */
    void on_file_entered(std::string &file_name);

    /**
     * Handle start game button
     */
    void on_start_game_button_clicked();

private:
    /**
     * A map file name
     */
    std::string map_file_name {};
};


#endif    // PACMAN_SCREENLOADGAME_H
