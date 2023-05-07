/**
 * @file menu_screen.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#include "screen_menu.h"

#include "widget_main_menu.h"

#include <QGridLayout>
#include <QLabel>

ScreenMenu::ScreenMenu(QWidget *parent) : QWidget(parent)
{
    auto menu_screen_layout = new QGridLayout(this);
    auto pacman_text = new QLabel("PacMan", this);

    QFont font = pacman_text->font();
    font.setPointSize(36); // Set the font size to a large value
    pacman_text->setFont(font);

    // Set stretches
    menu_screen_layout->setColumnStretch(0, 1);
    menu_screen_layout->setColumnStretch(1, 3);
    menu_screen_layout->setColumnStretch(2, 1);

    menu_screen_layout->setRowStretch(0, 2);
    menu_screen_layout->setRowStretch(1, 3);
    menu_screen_layout->setRowStretch(2, 1);

    // Create main menu and insert it to the layout
    auto main_menu = new MainMenu(this);
    menu_screen_layout->addWidget(main_menu, 1, 1, Qt::AlignCenter);
    menu_screen_layout->addWidget(pacman_text, 0, 1, Qt::AlignCenter);
    connect(main_menu, &MainMenu::clicked_new_game, this, &ScreenMenu::on_new_game_button);
    connect(main_menu, &MainMenu::clicked_load_game, this, &ScreenMenu::on_load_game_button);
    this->setLayout(menu_screen_layout);
}

void ScreenMenu::on_new_game_button()
{
    emit ScreenMenu::go_to_new_game_screen();
}

void ScreenMenu::on_load_game_button()
{
    emit ScreenMenu::go_to_load_game_screen();
}
