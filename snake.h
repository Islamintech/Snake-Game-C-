// ============================================================
//  snake.h
//  Snake class: holds the body segments, direction and the
//  movement / growth / shrink logic.
// ============================================================
#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include "common.h"

class Snake {
public:
    Snake();

    // Initialise a length-3 snake at head facing dir.
    void reset(const Point& head, Dir dir);

    // Try to change direction. Returns false if the requested
    // direction is the direct opposite of the current one
    // (Rule: reversing into yourself is an immediate game over).
    bool setDirection(Dir d);

    Dir   direction() const { return dir_; }
    Point head()      const { return body_.front(); }
    Point neck()      const;                  // segment right behind head
    int   length()    const { return static_cast<int>(body_.size()); }

    // Next head position given the current direction.
    Point nextHead() const;

    // Advance one cell. If grow is false the tail is removed.
    void advance(const Point& newHead, bool grow);
    // Remove one segment from the tail (poison). Caller checks length.
    void shrink();

    // Teleport the head to an arbitrary cell (gate exit) with a
    // new direction. If grow is false the tail is removed.
    void teleport(const Point& newHead, Dir newDir, bool grow);

    bool occupies(const Point& p) const;        // any segment
    bool bodyHit(const Point& p) const;         // any segment except head

    const std::deque<Point>& body() const { return body_; }

private:
    std::deque<Point> body_;   // front() == head
    Dir dir_;
};

#endif // SNAKE_H
