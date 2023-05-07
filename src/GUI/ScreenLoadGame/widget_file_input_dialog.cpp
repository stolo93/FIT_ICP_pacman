/**
 * @file widget_file_input_dialog.cpp
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */

#include "widget_file_input_dialog.h"

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>

MapFileInput::MapFileInput(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);

    auto open_file_button = new QPushButton("Choose a game file");
    connect(open_file_button, &QPushButton::clicked, this, &MapFileInput::on_open_file_button_clicked);

    layout->addStretch(1);
    layout->addWidget(open_file_button, 3);
    layout->addStretch(1);
}

void MapFileInput::on_open_file_button_clicked()
{
    auto qt_file_name = QFileDialog::getOpenFileName(this, "Open a game file", "", "All files (*)");
    auto file_name {qt_file_name.toStdString()};

    emit file_name_entered(file_name);
}
