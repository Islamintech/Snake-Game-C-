// ============================================================
//  item.h
//  Item + ItemManager classes.
//  An Item is a single growth (+) or poison (-) cell.
//  ItemManager keeps the board populated with a bounded number
//  of items and refreshes them on a timer.
// ============================================================
#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "common.h"

class Board;
class Snake;

class Item {
public:
    Item(const Point& pos, int type, long bornTick)
        : pos_(pos), type_(type), bornTick_(bornTick) {}

    Point pos()      const { return pos_; }
    int   type()     const { return type_; }      // CELL_GROWTH or CELL_POISON
    long  bornTick() const { return bornTick_; }

private:
    Point pos_;
    int   type_;
    long  bornTick_;
};

class ItemManager {
public:
    ItemManager();

    void reset(Board& board);

    // Called once per tick. Spawns/expires items and writes their
    // codes into the board grid.
    void update(Board& board, const Snake& snake, long tick);

    // If p holds an item, consume it and return its type
    // (CELL_GROWTH / CELL_POISON); returns 0 otherwise.
    int consumeAt(Board& board, const Point& p);

private:
    std::vector<Item> items_;
    long lastSpawnTick_;

    static const int  kMaxItems   = 5;   // max items on the map at once
    static const long kLifeTicks  = 40;  // ticks before an item expires
    static const long kSpawnEvery = 8;   // ticks between spawn attempts

    bool spawnOne(Board& board, const Snake& snake, long tick);
    void writeToBoard(Board& board) const;
};

#endif // ITEM_H
