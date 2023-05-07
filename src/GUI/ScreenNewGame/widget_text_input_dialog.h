/**
 * @file text_input_dialog.h
 * @author Samuel Stolarik
 * @date 2023-04-19
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_WIDGET_TEXT_INPUT_DIALOG_H
#define PACMAN_WIDGET_TEXT_INPUT_DIALOG_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class TextInputDialog
 */
class TextInputDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TextInputDialog(const QString &label_text, QWidget *parent = nullptr);

signals:
    /**
     * Emit text entered signal
     * @param text
     */
    void text_entered(const QString &text);

private slots:
    /**
     * Handle submit button clicked
     */
    void on_button_clicked();

private:
    /**
     * Line editor
     */
    QLineEdit *line_edit;
};


#endif    // PACMAN_WIDGET_TEXT_INPUT_DIALOG_H
