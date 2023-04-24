/**
 * @file screen_new_game.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#include "screen_new_game.h"

#include "widget_map_file_dialog.h"
#include "widget_text_input_dialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <iostream>
#include <string>

ScreenNewGame::ScreenNewGame(QWidget *parent) : QWidget(parent) {
    auto layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);
    layout->setColumnStretch(2, 1);

    input_dialog = new TextInputDialog("Enter user name:", this);
    auto input_file_dialog = new MapFileDialog(this);
    auto start_game_button = new QPushButton("Start game!");
    auto back_button = new QPushButton("Back");

    connect(back_button, &QPushButton::clicked, this, &ScreenNewGame::on_go_back_button);
    connect(input_dialog, &TextInputDialog::text_entered, this, &ScreenNewGame::on_text_entered);
    connect(input_file_dialog, &MapFileDialog::map_file_entered, this, &ScreenNewGame::on_file_name_entered);
    connect(start_game_button, &QPushButton::clicked, this, &ScreenNewGame::on_start_game_clicked);

    layout->addWidget(input_dialog, 1, 1);
    layout->addWidget(input_file_dialog, 2, 1);
    layout->addWidget(start_game_button, 2, 2, Qt::AlignBottom);
    layout->addWidget(back_button, 0, 0);
}

void ScreenNewGame::on_go_back_button() {
    emit ScreenNewGame::go_to_home();
}

void ScreenNewGame::on_text_entered(const QString &text) {
    user_name = text.toStdString();
}

void ScreenNewGame::on_file_name_entered(std::string &file_name) {
    map_file_name = file_name;
}

void ScreenNewGame::on_start_game_clicked() {
    emit this->start_game(user_name, map_file_name);
}