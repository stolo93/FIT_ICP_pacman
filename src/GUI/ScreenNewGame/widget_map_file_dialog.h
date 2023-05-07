/**
 * @file map_file_dialog.h
 * @author Samuel Stolarik
 * @date 2023-04-20
 * Project - ICP - project PacMan
 */
#ifndef PACMAN_MAPFILEDIALOG_H
#define PACMAN_MAPFILEDIALOG_H

#include <QWidget>

/**
 * @class MapFileDialog
 * @brief Dialog for choosing map file
 */
class MapFileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapFileDialog(QWidget *parent = nullptr);

signals:
    /**
     * Emit map file entered
     * @param map_file_name
     */
    void map_file_entered(std::string &map_file_name);

private slots:
    /**
     * Handle open file button
     */
    void on_open_file_button();
};


#endif    // PACMAN_MAPFILEDIALOG_H
