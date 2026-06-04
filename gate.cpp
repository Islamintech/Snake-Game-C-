// ============================================================
//  gate.cpp
//  Implementation of the Gate class.
// ============================================================
#include "gate.h"
#include "board.h"
#include <cstdlib>

Gate::Gate() : active_(false) {}

bool Gate::place(Board& board) {
    active_ = false;
    std::vector<Point> walls = board.wallCells();
    if (walls.size() < 2) return false;

    // Pick two distinct normal-wall cells.
    int ia = std::rand() % static_cast<int>(walls.size());
    int ib = std::rand() % static_cast<int>(walls.size());
    for (int guard = 0; ib == ia && guard < 50; ++guard) {
        ib = std::rand() % static_cast<int>(walls.size());
    }
    if (ia == ib) return false;

    a_ = walls[ia];
    b_ = walls[ib];
    board.set(a_.y, a_.x, CELL_GATE);
    board.set(b_.y, b_.x, CELL_GATE);
    active_ = true;
    return true;
}

bool Gate::isGate(const Point& p) const {
    return active_ && (p == a_ || p == b_);
}

Dir Gate::edgeExitDir(const Board& board, const Point& g) const {
    if (g.y == 0)                    return Dir::Down;
    if (g.y == board.height() - 1)   return Dir::Up;
    if (g.x == 0)                    return Dir::Right;
    if (g.x == board.width() - 1)    return Dir::Left;
    return Dir::None;  // interior wall
}

bool Gate::resolve(const Board& board, const Point& enterPos,
                   Dir entryDir, Point& outCell, Dir& outDir) const {
    if (!active_) return false;

    // Determine which gate is the exit.
    Point exitGate = (enterPos == a_) ? b_ : a_;

    // Build the priority list of candidate exit directions.
    Dir edge = edgeExitDir(board, exitGate);
    Dir candidates[4];
    int n = 0;
    if (edge != Dir::None) {
        // Edge gate: direction is forced, but still fall back if blocked.
        candidates[n++] = edge;
        candidates[n++] = clockwise(edge);
        candidates[n++] = counterClockwise(edge);
        candidates[n++] = opposite(edge);
    } else {
        // Interior gate: keep going, else turn, else reverse.
        candidates[n++] = entryDir;
        candidates[n++] = clockwise(entryDir);
        candidates[n++] = counterClockwise(entryDir);
        candidates[n++] = opposite(entryDir);
    }

    for (int i = 0; i < n; ++i) {
        Point dv = delta(candidates[i]);
        Point cell(exitGate.y + dv.y, exitGate.x + dv.x);
        if (!board.inBounds(cell.y, cell.x)) continue;
        if (board.isWall(cell.y, cell.x))    continue;
        if (board.at(cell.y, cell.x) == CELL_GATE) continue;
        outCell = cell;
        outDir  = candidates[i];
        return true;
    }
    return false;
}
