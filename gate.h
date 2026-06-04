// ============================================================
//  gate.h
//  Gate class: a pair of linked gate cells placed on walls.
//  Handles the directional exit logic when the snake passes
//  through a gate.
// ============================================================
#ifndef GATE_H
#define GATE_H

#include "common.h"

class Board;

class Gate {
public:
    Gate();

    // Pick a random pair of normal-wall cells and stamp them as
    // gates on the board. Returns false if a pair cannot be found.
    bool place(Board& board);

    bool active() const { return active_; }

    // True if p is one of the two gate cells.
    bool isGate(const Point& p) const;

    // Given the entering cell (enterPos == one gate) and the
    // entry direction, compute the head's exit cell and the new
    // direction. Returns true on success.
    bool resolve(const Board& board, const Point& enterPos,
                 Dir entryDir, Point& outCell, Dir& outDir) const;

    Point a() const { return a_; }
    Point b() const { return b_; }

private:
    Point a_;
    Point b_;
    bool  active_;

    // Forced exit direction for an edge gate, or Dir::None if the
    // gate is on an interior wall.
    Dir edgeExitDir(const Board& board, const Point& g) const;
};

#endif // GATE_H
