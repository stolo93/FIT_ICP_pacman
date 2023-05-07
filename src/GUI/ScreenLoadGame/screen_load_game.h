/**
 * @file scree_load_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */


#ifndef PACMAN_SCREENLOADGAME_H
#define PACMAN_SCREENLOADGAME_H

#include <QWidget>

class ScreenLoadGame : public QWidget {
Q_OBJECT

public:
    explicit ScreenLoadGame(QWidget *parent = nullptr);

signals:

    void go_to_home();

    void replay_game(std::string &file_name_map);

private slots:

    void on_go_back_button();

    void on_file_entered(std::string &file_name);

    void on_start_game_button_clicked();

private:
    std::string map_file_name{};
};


#endif    // PACMAN_SCREENLOADGAME_H
