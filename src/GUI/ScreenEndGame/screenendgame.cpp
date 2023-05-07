//
// Created by samuel on 7.5.2023.
//

#include "screenendgame.h"
#include <QLabel>
#include <QVBoxLayout>

namespace view
{
    ScreenEndGame::ScreenEndGame(QWidget *parent) : QWidget(parent){
        auto layout = new QVBoxLayout(this);
        label = new QLabel("", this);
        QFont font = label->font();
        font.setPointSize(36); // Set the font size to a large value
        label->setFont(font);

        layout->addWidget(label, Qt::AlignCenter);
    }

    void ScreenEndGame::on_set_text(const QString &text) {
        label->setText(text);
    }
}