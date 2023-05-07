/**
 * @file livesdisplay.h
 * @author Samuel Stolarik
 * @date 2023-05-05
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_LIVESDISPLAY_H
#define PACMAN_LIVESDISPLAY_H
#include <QLabel>
#include <QWidget>

namespace view
{

/**
 * @class LivesDisplayer
 * @brief Text only lives counter
 */
class LivesDisplayer : public QWidget
{
    Q_OBJECT
public:
    LivesDisplayer(QWidget *parent = nullptr, uint8_t lives = 0);

    /**
     * Update lives count
     * @param lives
     */
    void update_lives_count(uint8_t lives);

private:
    /**
     * Lives count
     */
    uint8_t lives_count;

    /**
     * Lives count label
     */
    QLabel *lives_label;
};

}    // namespace view

#endif    // PACMAN_LIVESDISPLAY_H
