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

/**
 * @class ScreenNewGame
 */
class ScreenNewGame : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenNewGame(QWidget *parent = nullptr);

signals:
    /**
     * Emit go to home
     */
    void go_to_home();

    /**
     * emit start game
     * @param user_name
     * @param file_name_map
     */
    void start_game(std::string &user_name, std::string &file_name_map);

private slots:
    /**
     * Handle go back button
     */
    void on_go_back_button();

    /**
     * Handle text entered
     * @param text
     */
    void on_text_entered(const QString &text);

    /**
     * Handle map file entered
     * @param file_name
     */
    void on_file_name_entered(std::string &file_name);

    /**
     * Handle start game clicked
     */
    void on_start_game_clicked();

private:
    /**
     * User name input dialog
     */
    TextInputDialog *input_dialog {};

    /**
     * user name
     */
    std::string user_name {};

    /**
     * Map file name
     */
    std::string map_file_name {};
};


#endif    // PACMAN_SCREENNEWGAME_H
