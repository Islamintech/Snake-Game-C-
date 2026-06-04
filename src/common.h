// ============================================================
//  common.h
//  Shared constants, enums and small value types for the
//  Snake Game term project (C++ / curses).
//  Author : Kookmin University - C++ Term Project
// ============================================================
#ifndef COMMON_H
#define COMMON_H

// ---- Map cell codes (as defined by the project spec) -------
enum CellType {
    CELL_EMPTY       = 0,   // empty space
    CELL_WALL        = 1,   // normal wall (gate may appear here)
    CELL_IMMUNE_WALL = 2,   // immune wall / corner (no gate here)
    CELL_SNAKE_HEAD  = 3,   // snake head
    CELL_SNAKE_BODY  = 4,   // snake body
    CELL_GROWTH      = 5,   // growth item (+1 length)
    CELL_POISON      = 6,   // poison item (-1 length)
    CELL_GATE        = 7    // gate
};

// ---- curses colour pair identifiers ------------------------
enum ColorPairId {
    CP_DEFAULT = 1,
    CP_WALL,
    CP_IMMUNE,
    CP_HEAD,
    CP_BODY,
    CP_GROWTH,
    CP_POISON,
    CP_GATE,
    CP_TEXT
};

// ---- Movement direction ------------------------------------
enum class Dir { Up, Down, Left, Right, None };

// ---- A simple board coordinate (row = y, col = x) ----------
struct Point {
    int y;
    int x;

    Point() : y(0), x(0) {}
    Point(int yy, int xx) : y(yy), x(xx) {}

    bool operator==(const Point& o) const { return y == o.y && x == o.x; }
    bool operator!=(const Point& o) const { return !(*this == o); }
};

// ---- Helpers for directions --------------------------------
inline Point delta(Dir d) {
    switch (d) {
        case Dir::Up:    return Point(-1, 0);
        case Dir::Down:  return Point( 1, 0);
        case Dir::Left:  return Point( 0,-1);
        case Dir::Right: return Point( 0, 1);
        default:         return Point( 0, 0);
    }
}

inline Dir opposite(Dir d) {
    switch (d) {
        case Dir::Up:    return Dir::Down;
        case Dir::Down:  return Dir::Up;
        case Dir::Left:  return Dir::Right;
        case Dir::Right: return Dir::Left;
        default:         return Dir::None;
    }
}

// clockwise 90-degree turn
inline Dir clockwise(Dir d) {
    switch (d) {
        case Dir::Up:    return Dir::Right;
        case Dir::Right: return Dir::Down;
        case Dir::Down:  return Dir::Left;
        case Dir::Left:  return Dir::Up;
        default:         return Dir::None;
    }
}

// counter-clockwise 90-degree turn
inline Dir counterClockwise(Dir d) {
    switch (d) {
        case Dir::Up:    return Dir::Left;
        case Dir::Left:  return Dir::Down;
        case Dir::Down:  return Dir::Right;
        case Dir::Right: return Dir::Up;
        default:         return Dir::None;
    }
}

#endif // COMMON_H
