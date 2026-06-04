// ============================================================
//  scoreboard.cpp
//  Implementation of the ScoreBoard class.
// ============================================================
#include "scoreboard.h"
#include "common.h"
#include <curses.h>

ScoreBoard::ScoreBoard()
    : curLength_(3), maxLength_(3), growth_(0),
      poison_(0), gate_(0), seconds_(0) {
    mission_ = missionFor(0);
}

Mission ScoreBoard::missionFor(int stage) {
    // Progressive difficulty across the four stages.
    switch (stage) {
        case 0: return Mission{ 5, 2, 0, 0 };
        case 1: return Mission{ 7, 3, 1, 1 };
        case 2: return Mission{ 9, 4, 2, 2 };
        case 3: return Mission{ 11, 5, 2, 3 };
        default: return Mission{ 5, 2, 0, 0 };
    }
}

void ScoreBoard::reset(int stage) {
    curLength_ = 3;
    maxLength_ = 3;
    growth_    = 0;
    poison_    = 0;
    gate_      = 0;
    seconds_   = 0;
    mission_   = missionFor(stage);
}

void ScoreBoard::onGrowth() { ++growth_; }
void ScoreBoard::onPoison() { ++poison_; }
void ScoreBoard::onGate()   { ++gate_;   }

void ScoreBoard::setLength(int len) {
    curLength_ = len;
    if (len > maxLength_) maxLength_ = len;
}

void ScoreBoard::tickSeconds(int elapsedSeconds) {
    seconds_ = elapsedSeconds;
}

bool ScoreBoard::missionComplete() const {
    return maxLength_ >= mission_.maxLength &&
           growth_    >= mission_.growth    &&
           poison_    >= mission_.poison    &&
           gate_      >= mission_.gate;
}

void ScoreBoard::draw(int originX, int stage) const {
    int y = 1;
    attron(COLOR_PAIR(CP_TEXT));

    mvprintw(y++, originX, "+----------------+");
    mvprintw(y++, originX, "|   SCORE BOARD  |");
    mvprintw(y++, originX, "+----------------+");
    mvprintw(y++, originX, "| Stage : %-6d |", stage + 1);
    mvprintw(y++, originX, "| B: %3d / %-3d   |", curLength_, maxLength_);
    mvprintw(y++, originX, "| +: %-11d |", growth_);
    mvprintw(y++, originX, "| -: %-11d |", poison_);
    mvprintw(y++, originX, "| G: %-11d |", gate_);
    mvprintw(y++, originX, "| Time: %3d s    |", seconds_);
    mvprintw(y++, originX, "+----------------+");

    ++y;
    mvprintw(y++, originX, "+----------------+");
    mvprintw(y++, originX, "|    MISSION     |");
    mvprintw(y++, originX, "+----------------+");

    char ok;
    ok = (maxLength_ >= mission_.maxLength) ? 'v' : ' ';
    mvprintw(y++, originX, "| B: %-3d     (%c) |", mission_.maxLength, ok);
    ok = (growth_ >= mission_.growth) ? 'v' : ' ';
    mvprintw(y++, originX, "| +: %-3d     (%c) |", mission_.growth, ok);
    ok = (poison_ >= mission_.poison) ? 'v' : ' ';
    mvprintw(y++, originX, "| -: %-3d     (%c) |", mission_.poison, ok);
    ok = (gate_ >= mission_.gate) ? 'v' : ' ';
    mvprintw(y++, originX, "| G: %-3d     (%c) |", mission_.gate, ok);
    mvprintw(y++, originX, "+----------------+");

    ++y;
    mvprintw(y++, originX, "Arrows: move");
    mvprintw(y++, originX, "P: pause  Q: quit");

    attroff(COLOR_PAIR(CP_TEXT));
}
