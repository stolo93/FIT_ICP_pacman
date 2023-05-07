## Qt C++ Pacman

This is a Pacman implementation in C++ using the Qt framework. The game offers two main menu options, New Game and Load
Game, along with three in-game modes: Play, Replay, and Pause. The player has to collect all keys and reach the target
square without touching the ghosts more than two times. Our implementation features interactive elements, such as a life
counter and display, as well as the ability to switch freely between Replay and Play modes, enabling players to continue
the game from any point. The ghost movement is powered by a greedy path search AI algorithm.

## Main Menu Options

#### New Game

In the New Game option, the user is prompted to enter their username and select a file containing a map. After pressing
the Start Game button, the user is taken to the game screen.

#### Load Game

In the Load Game option, the user is prompted to choose a previously saved .log file. After selecting the file and
pressing the Start Game button, the user is taken to the game screen.

## In-Game Modes

#### Play

This is the classic gameplay mode where the player controls the Pacman character using the arrow keys.

#### Replay

This mode allows players to replay a log file or rewind their current game. The player can control the replay by holding
the left and right arrow keys.

#### Pause

This mode simply pauses the game.

## Controls

Players can switch between modes using the buttons at the top of the screen or by pressing the following keys:

* Esc: Pause
* R: Replay
* P: Play

To control the gameplay, use the arrow keys to move the Pacman character. In Replay mode, hold the left and right arrow
keys to control the playback.

## Logging

Upon finishing a game in New Game mode, a .log file with the player's nickname is created. This log file can be used in
the Load Game option to replay the game.

## Interactive Features

#### Life Counter and Display

The game includes a life counter and display, showing the remaining number of lives for the player.

#### Replay and Play Switching

Our implementation enables players to switch freely between Replay and Play modes, allowing them to continue the game
from any point.

#### AI Algorithm

The ghost movement is powered by a greedy path search AI algorithm, providing challenging gameplay for players.

## Building
To build the project first install qt 5 then run `make all` in the root directory you executable should located in `./build/pacman`. Enjoy.

