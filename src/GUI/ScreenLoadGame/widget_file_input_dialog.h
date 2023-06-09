/**
 * @file widget_file_input_dialog.h
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_WIDGETFILEINPUTDIALOG_H
#define PACMAN_WIDGETFILEINPUTDIALOG_H

#include <QWidget>

/**
 * @class MapFileInput
 * @brief Dialog for choosing map file
 */
class MapFileInput : public QWidget
{
    Q_OBJECT

public:
    explicit MapFileInput(QWidget *parent = nullptr);

signals:
    /**
     * File name entered
     * @param file_name
     */
    void file_name_entered(std::string &file_name);

private slots:
    /**
     * Open dialog
     */
    void on_open_file_button_clicked();
};


#endif    // PACMAN_WIDGETFILEINPUTDIALOG_H
