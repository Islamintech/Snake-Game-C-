// ============================================================
//  board.cpp
//  Implementation of the Board class.
// ============================================================
#include "board.h"
#include <curses.h>

namespace {
    const int kStageCount = 4;

    // Interior wall segments per stage, expressed as (y,x) pairs.
    // The outer frame is generated separately by buildBorder().
    // Coordinates assume a 21x21 board.
    void applyStageObstacles(std::vector<std::vector<int>>& g, int stage) {
        const int H = static_cast<int>(g.size());
        const int W = static_cast<int>(g[0].size());

        switch (stage) {
            case 0:
                // Stage 1: empty arena, just the border.
                break;

            case 1: {
                // Stage 2: two horizontal bars.
                for (int x = 4; x <= W - 5; ++x) {
                    g[5][x]      = CELL_WALL;
                    g[H - 6][x]  = CELL_WALL;
                }
                break;
            }

            case 2: {
                // Stage 3: a central cross.
                for (int x = 4; x <= W - 5; ++x) g[H / 2][x] = CELL_WALL;
                for (int y = 4; y <= H - 5; ++y) g[y][W / 2] = CELL_WALL;
                // open the very centre so the snake can pass
                g[H / 2][W / 2] = CELL_EMPTY;
                break;
            }

            case 3: {
                // Stage 4: four interior blocks (chambers).
                for (int y = 4; y <= 7; ++y) {
                    g[y][6]      = CELL_WALL;
                    g[y][W - 7]  = CELL_WALL;
                }
                for (int y = H - 8; y <= H - 5; ++y) {
                    g[y][6]      = CELL_WALL;
                    g[y][W - 7]  = CELL_WALL;
                }
                break;
            }

            default:
                break;
        }
    }
}

Board::Board(int height, int width)
    : height_(height), width_(width),
      grid_(height, std::vector<int>(width, CELL_EMPTY)),
      startHead_(height / 2, width / 2) {}

int Board::stageCount() const { return kStageCount; }

void Board::buildBorder() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            grid_[y][x] = CELL_EMPTY;
        }
    }
    for (int x = 0; x < width_; ++x) {
        grid_[0][x]           = CELL_WALL;
        grid_[height_ - 1][x] = CELL_WALL;
    }
    for (int y = 0; y < height_; ++y) {
        grid_[y][0]          = CELL_WALL;
        grid_[y][width_ - 1] = CELL_WALL;
    }
    // Corners become immune walls (gates may not appear there).
    grid_[0][0]                       = CELL_IMMUNE_WALL;
    grid_[0][width_ - 1]              = CELL_IMMUNE_WALL;
    grid_[height_ - 1][0]            = CELL_IMMUNE_WALL;
    grid_[height_ - 1][width_ - 1]   = CELL_IMMUNE_WALL;
}

void Board::loadStage(int stage) {
    buildBorder();
    applyStageObstacles(grid_, stage);

    // Place the starting head where there is guaranteed free space.
    startHead_ = Point(height_ / 2, 3);
    // Nudge if the chosen cell is occupied by an obstacle.
    if (grid_[startHead_.y][startHead_.x] != CELL_EMPTY) {
        startHead_ = Point(2, 3);
    }
}

bool Board::inBounds(int y, int x) const {
    return y >= 0 && y < height_ && x >= 0 && x < width_;
}

int Board::at(int y, int x) const {
    if (!inBounds(y, x)) return CELL_WALL;
    return grid_[y][x];
}

void Board::set(int y, int x, int value) {
    if (inBounds(y, x)) grid_[y][x] = value;
}

bool Board::isWall(int y, int x) const {
    int c = at(y, x);
    return c == CELL_WALL || c == CELL_IMMUNE_WALL;
}

bool Board::isImmuneWall(int y, int x) const {
    return at(y, x) == CELL_IMMUNE_WALL;
}

std::vector<Point> Board::wallCells() const {
    std::vector<Point> out;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[y][x] == CELL_WALL) out.push_back(Point(y, x));
        }
    }
    return out;
}

std::vector<Point> Board::emptyCells() const {
    std::vector<Point> out;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[y][x] == CELL_EMPTY) out.push_back(Point(y, x));
        }
    }
    return out;
}

void Board::draw() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int c = grid_[y][x];
            switch (c) {
                case CELL_WALL:
                    attron(COLOR_PAIR(CP_WALL));
                    mvaddch(y, x * 2, ' '); mvaddch(y, x * 2 + 1, ' ');
                    attroff(COLOR_PAIR(CP_WALL));
                    break;
                case CELL_IMMUNE_WALL:
                    attron(COLOR_PAIR(CP_IMMUNE));
                    mvaddch(y, x * 2, ' '); mvaddch(y, x * 2 + 1, ' ');
                    attroff(COLOR_PAIR(CP_IMMUNE));
                    break;
                default:
                    attron(COLOR_PAIR(CP_DEFAULT));
                    mvaddch(y, x * 2, ' '); mvaddch(y, x * 2 + 1, ' ');
                    attroff(COLOR_PAIR(CP_DEFAULT));
                    break;
            }
        }
    }
}
