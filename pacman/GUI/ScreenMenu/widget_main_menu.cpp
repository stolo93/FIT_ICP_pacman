/**
 * @file main_menu.cpp
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#include "widget_main_menu.h"

#include <QPushButton>
#include <QVBoxLayout>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    auto menu_layout = new QVBoxLayout(this);

    auto button_new_game = new QPushButton("New game");
    button_new_game->setFixedSize(120, 60);

    auto button_load_game = new QPushButton("Load game");
    button_load_game->setFixedSize(120, 60);

    menu_layout->addWidget(button_new_game);
    menu_layout->setAlignment(button_new_game, Qt::AlignHCenter | Qt::AlignTop);

    menu_layout->addStretch(1);

    menu_layout->addWidget(button_load_game);
    menu_layout->setAlignment(button_load_game, Qt::AlignHCenter | Qt::AlignTop);

    connect(button_new_game, &QPushButton::clicked, this, &MainMenu::on_new_game_button);
    connect(button_load_game, &QPushButton::clicked, this, &MainMenu::on_load_game_button);
    this->setLayout(menu_layout);
}

void MainMenu::on_new_game_button()
{
    emit MainMenu::clicked_new_game();
}

void MainMenu::on_load_game_button()
{
    emit MainMenu::clicked_load_game();
}