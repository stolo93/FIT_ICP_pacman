/**
 * @file parse.h
 * @date 2023-04-27
 * Project - ICP - project PacMan
 */

#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

#include <QKeyEvent>

namespace ctl
{

enum class ControllerState { StateReplay, StateGameplay, StatePaused, StateNotSetup };

enum class GameResult {Win, Loose};

enum class KeyEventType { KeyPress, KeyRelease };

struct KeyEvent {
    KeyEventType event_type;
    Qt::Key key;
};

}    // namespace ctl

#endif    // PACMAN_PACMAN_H
