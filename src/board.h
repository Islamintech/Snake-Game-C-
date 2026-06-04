// ============================================================
//  board.h
//  Board class: owns the 2D map grid, builds per-stage layouts
//  and renders the play field with curses.
// ============================================================
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "common.h"

class Board {
public:
    Board(int height, int width);

    // Build the map for the given stage index (0-based).
    void loadStage(int stage);

    int  height() const { return height_; }
    int  width()  const { return width_;  }
    int  stageCount() const;

    // Cell access (const + mutating).
    int  at(int y, int x) const;
    void set(int y, int x, int value);

    bool inBounds(int y, int x) const;
    bool isWall(int y, int x) const;          // wall or immune wall
    bool isImmuneWall(int y, int x) const;

    // Suggested starting head position / direction for the snake.
    Point startHead() const { return startHead_; }
    Dir   startDir()  const { return Dir::Right; }

    // Collect all normal-wall cells eligible to host a gate.
    std::vector<Point> wallCells() const;
    // Collect all empty cells (for item placement).
    std::vector<Point> emptyCells() const;

    void draw() const;   // render the static map (walls etc.)

private:
    int height_;
    int width_;
    std::vector<std::vector<int>> grid_;
    Point startHead_;

    void buildBorder();  // outer frame: corners immune, edges wall
};

#endif // BOARD_H
