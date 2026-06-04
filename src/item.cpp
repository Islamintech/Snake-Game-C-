// ============================================================
//  item.cpp
//  Implementation of Item / ItemManager.
// ============================================================
#include "item.h"
#include "board.h"
#include "snake.h"
#include <cstdlib>

ItemManager::ItemManager() : lastSpawnTick_(0) {}

void ItemManager::reset(Board& /*board*/) {
    items_.clear();
    lastSpawnTick_ = 0;
}

void ItemManager::writeToBoard(Board& board) const {
    for (const Item& it : items_) {
        board.set(it.pos().y, it.pos().x, it.type());
    }
}

bool ItemManager::spawnOne(Board& board, const Snake& snake, long tick) {
    std::vector<Point> cells = board.emptyCells();
    if (cells.empty()) return false;

    // Try a handful of random cells that are not on the snake.
    for (int attempt = 0; attempt < 20; ++attempt) {
        const Point& p = cells[std::rand() % cells.size()];
        if (snake.occupies(p)) continue;

        bool taken = false;
        for (const Item& it : items_) {
            if (it.pos() == p) { taken = true; break; }
        }
        if (taken) continue;

        // 60% growth, 40% poison.
        int type = (std::rand() % 100 < 60) ? CELL_GROWTH : CELL_POISON;
        items_.push_back(Item(p, type, tick));
        return true;
    }
    return false;
}

void ItemManager::update(Board& board, const Snake& snake, long tick) {
    // Expire old items (clear their cell first).
    std::vector<Item> kept;
    kept.reserve(items_.size());
    for (const Item& it : items_) {
        if (tick - it.bornTick() >= kLifeTicks) {
            board.set(it.pos().y, it.pos().x, CELL_EMPTY);
        } else {
            kept.push_back(it);
        }
    }
    items_.swap(kept);

    // Periodically top up the board with new items.
    if (tick - lastSpawnTick_ >= kSpawnEvery) {
        lastSpawnTick_ = tick;
        if (static_cast<int>(items_.size()) < kMaxItems) {
            spawnOne(board, snake, tick);
        }
    }

    writeToBoard(board);
}

int ItemManager::consumeAt(Board& board, const Point& p) {
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].pos() == p) {
            int type = items_[i].type();
            board.set(p.y, p.x, CELL_EMPTY);
            items_.erase(items_.begin() + i);
            return type;
        }
    }
    return 0;
}
