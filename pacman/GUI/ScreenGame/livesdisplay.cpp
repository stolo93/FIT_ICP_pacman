//
// Created by samuel on 7.5.2023.
//

#include "livesdisplay.h"
#include <QLabel>
#include <QHBoxLayout>

namespace view {
    LivesDisplayer::LivesDisplayer(QWidget *parent, uint8_t lives) : QWidget(parent), lives_count(lives), lives_label(new QLabel()) {
        auto layout = new QHBoxLayout(this);
        auto label_text = new QLabel("Remaining lives: ", this);
        update_lives_count(lives);

        layout->addWidget(label_text, Qt::AlignRight | Qt::AlignVCenter);
        layout->addWidget(lives_label, Qt::AlignLeft | Qt::AlignVCenter);
    }

    void LivesDisplayer::update_lives_count(uint8_t lives) {
        if (! lives_label){return;}
        this->lives_count = lives;
        auto lives_text = QString::fromStdString(std::to_string(lives_count));
        lives_label->setText(lives_text);
    }
}