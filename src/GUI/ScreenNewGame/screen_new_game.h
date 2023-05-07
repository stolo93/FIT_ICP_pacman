/**
 * @file screen_new_game.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_SCREENNEWGAME_H
#define PACMAN_SCREENNEWGAME_H

#include "widget_text_input_dialog.h"

#include <QWidget>
#include <string>

class ScreenNewGame : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenNewGame(QWidget *parent = nullptr);

signals:
    void go_to_home();
    void start_game(std::string &user_name, std::string &file_name_map);

private slots:
    void on_go_back_button();
    void on_text_entered(const QString &text);
    void on_file_name_entered(std::string &file_name);
    void on_start_game_clicked();

private:
    TextInputDialog *input_dialog {};
    std::string user_name {};
    std::string map_file_name {};
};


#endif    // PACMAN_SCREENNEWGAME_H
