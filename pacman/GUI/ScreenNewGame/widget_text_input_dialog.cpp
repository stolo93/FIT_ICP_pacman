/**
 * @file text_input_dialog.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#include "widget_text_input_dialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

TextInputDialog::TextInputDialog(const QString &label_text, QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    auto dialog_widget = new QWidget(this);
    auto dialog_layout = new QHBoxLayout(dialog_widget);
    dialog_layout->setMargin(0);

    line_edit = new QLineEdit();
    auto submit_button = new QPushButton("Submit");
    auto label = new QLabel(label_text);

    dialog_layout->addWidget(line_edit);
    dialog_layout->addWidget(submit_button);

    layout->addWidget(label, 1, Qt::AlignBottom | Qt::AlignLeft);
    layout->addWidget(dialog_widget, 1, Qt::AlignTop | Qt::AlignLeft);

    // Connect submit_button to on_button_clicked slot
    connect(submit_button, &QPushButton::clicked, this, &TextInputDialog::on_button_clicked);
}

void TextInputDialog::on_button_clicked()
{
    QString text = line_edit->text();
    if (! text.isEmpty()) { emit text_entered(text); }
}