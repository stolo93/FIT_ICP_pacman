/**
 * @file map_file_dialog.cpp
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */

#include "widget_map_file_dialog.h"

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

MapFileDialog::MapFileDialog(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    auto button = new QPushButton("Chose a map file");
    layout->addWidget(button, 1, Qt::AlignVCenter | Qt::AlignTop);

    connect(button, &QPushButton::clicked, this, &MapFileDialog::on_open_file_button);
}

void MapFileDialog::on_open_file_button()
{
    auto map_file = QFileDialog::getOpenFileName(this, "Open a map file", "", "All files (*)");
    if (! map_file.isEmpty()) {
        std::string file_name {map_file.toStdString()};
        emit MapFileDialog::map_file_entered(file_name);
    }
}
