// ============================================================
//  snake.cpp
//  Implementation of the Snake class.
// ============================================================
#include "snake.h"
#include <cstddef>

Snake::Snake() : dir_(Dir::Right) {}

void Snake::reset(const Point& head, Dir dir) {
    body_.clear();
    dir_ = dir;
    Point back = delta(opposite(dir));
    // Build a length-3 snake: head, then two segments trailing behind.
    body_.push_back(head);
    body_.push_back(Point(head.y + back.y,     head.x + back.x));
    body_.push_back(Point(head.y + back.y * 2, head.x + back.x * 2));
}

Point Snake::neck() const {
    if (body_.size() >= 2) return body_[1];
    return body_.front();
}

bool Snake::setDirection(Dir d) {
    if (d == Dir::None) return true;
    if (d == opposite(dir_)) return false;   // reversing is illegal
    dir_ = d;
    return true;
}

Point Snake::nextHead() const {
    Point h = body_.front();
    Point dv = delta(dir_);
    return Point(h.y + dv.y, h.x + dv.x);
}

void Snake::advance(const Point& newHead, bool grow) {
    body_.push_front(newHead);
    if (!grow) body_.pop_back();
}

void Snake::shrink() {
    if (!body_.empty()) body_.pop_back();
}

void Snake::teleport(const Point& newHead, Dir newDir, bool grow) {
    dir_ = newDir;
    body_.push_front(newHead);
    if (!grow) body_.pop_back();
}

bool Snake::occupies(const Point& p) const {
    for (const Point& s : body_) if (s == p) return true;
    return false;
}

bool Snake::bodyHit(const Point& p) const {
    for (size_t i = 1; i < body_.size(); ++i) if (body_[i] == p) return true;
    return false;
}
