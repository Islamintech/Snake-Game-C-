// ============================================================
//  game.h
//  Game class: top-level controller. Owns the board, snake,
//  items, gate and scoreboard, and runs the main game loop.
// ============================================================
#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "snake.h"
#include "item.h"
#include "gate.h"
#include "scoreboard.h"

class Game {
public:
    Game();
    ~Game();

    void run();   // initialise curses, run loop, clean up

private:
    Board      board_;
    Snake      snake_;
    ItemManager items_;
    Gate       gate_;
    ScoreBoard score_;

    int  stage_;
    long tick_;
    bool running_;
    bool paused_;

    static const int  kHeight     = 21;
    static const int  kWidth      = 21;
    static const int  kTickMs     = 250;   // game speed (ms per step)
    static const int  kMinLength  = 3;     // below this -> game over

    void initCurses();
    void endCurses();

    void startStage(int stage);
    bool step();                 // one logic step; false -> stop loop
    void handleInput();
    void render() const;
    void drawSnake() const;

    // result screens
    bool showResult(const char* title, const char* subtitle);

    Dir  pendingDir_;            // direction requested this tick
};

#endif // GAME_H
