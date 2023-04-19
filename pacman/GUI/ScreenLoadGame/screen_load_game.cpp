/**
 * @file scree_load_game.cpp
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */

#include "screen_load_game.h"

#include "widget_file_input_dialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <iostream>

ScreenLoadGame::ScreenLoadGame(QWidget *parent) : QWidget(parent)
{
    auto layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);
    layout->setColumnStretch(2, 1);

    auto start_game_button = new QPushButton("Start game!");
    auto file_dialog = new MapFileInput();
    auto back_button = new QPushButton("Back");

    connect(back_button, &QPushButton::clicked, this, &ScreenLoadGame::on_go_back_button);
    connect(file_dialog, &MapFileInput::file_name_entered, this, &ScreenLoadGame::on_file_entered);
    connect(start_game_button, &QPushButton::clicked, this, &ScreenLoadGame::on_start_game_button_clicked);

    layout->addWidget(back_button, 0, 0);
    layout->addWidget(file_dialog, 1, 1);
    layout->addWidget(start_game_button, 2, 2, Qt::AlignBottom);
}

void ScreenLoadGame::on_go_back_button()
{
    emit ScreenLoadGame::go_to_home();
}

void ScreenLoadGame::on_file_entered(std::string &file_name)
{
    map_file_name = file_name;
}

void ScreenLoadGame::on_start_game_button_clicked()
{
    // TODO connect controller
    std::cout << "Start game" << '\n';
    emit this->start_game(map_file_name);
}
