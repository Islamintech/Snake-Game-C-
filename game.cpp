// ============================================================
//  game.cpp
//  Implementation of the Game controller and main loop.
// ============================================================
#include "game.h"
#include <curses.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

namespace {
    void drawCell(int y, int x, char sym, int colorPair) {
        attron(COLOR_PAIR(colorPair));
        mvaddch(y, x * 2,     sym);
        mvaddch(y, x * 2 + 1, ' ');
        attroff(COLOR_PAIR(colorPair));
    }
}

Game::Game()
    : board_(kHeight, kWidth), stage_(0), tick_(0),
      running_(false), paused_(false), pendingDir_(Dir::None) {}

Game::~Game() {}

void Game::initCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(CP_DEFAULT, COLOR_WHITE,   COLOR_BLACK);
        init_pair(CP_WALL,    COLOR_WHITE,   COLOR_WHITE);
        init_pair(CP_IMMUNE,  COLOR_BLACK,   COLOR_YELLOW);
        init_pair(CP_HEAD,    COLOR_BLACK,   COLOR_GREEN);
        init_pair(CP_BODY,    COLOR_BLACK,   COLOR_CYAN);
        init_pair(CP_GROWTH,  COLOR_GREEN,   COLOR_BLACK);
        init_pair(CP_POISON,  COLOR_RED,     COLOR_BLACK);
        init_pair(CP_GATE,    COLOR_MAGENTA, COLOR_BLACK);
        init_pair(CP_TEXT,    COLOR_WHITE,   COLOR_BLACK);
    }
}

void Game::endCurses() {
    endwin();
}

void Game::startStage(int stage) {
    stage_  = stage;
    tick_   = 0;
    paused_ = false;
    pendingDir_ = Dir::None;

    board_.loadStage(stage);
    snake_.reset(board_.startHead(), board_.startDir());
    items_.reset(board_);
    gate_.place(board_);
    score_.reset(stage);
    score_.setLength(snake_.length());
}

void Game::handleInput() {
    int ch;
    while ((ch = getch()) != ERR) {
        switch (ch) {
            case KEY_UP:    pendingDir_ = Dir::Up;    break;
            case KEY_DOWN:  pendingDir_ = Dir::Down;  break;
            case KEY_LEFT:  pendingDir_ = Dir::Left;  break;
            case KEY_RIGHT: pendingDir_ = Dir::Right; break;
            case 'w': case 'W': pendingDir_ = Dir::Up;    break;
            case 's': case 'S': pendingDir_ = Dir::Down;  break;
            case 'a': case 'A': pendingDir_ = Dir::Left;  break;
            case 'd': case 'D': pendingDir_ = Dir::Right; break;
            case 'p': case 'P': paused_ = !paused_;       break;
            case 'q': case 'Q': running_ = false;         break;
            default: break;
        }
    }
}

bool Game::step() {
    // Apply the queued direction; reversing into the neck is fatal.
    if (!snake_.setDirection(pendingDir_)) {
        return false;   // signalled as game over by caller
    }

    Point nh = snake_.nextHead();

    // --- Gate handling ------------------------------------------------
    if (gate_.isGate(nh)) {
        Point exitCell;
        Dir   exitDir;
        if (gate_.resolve(board_, nh, snake_.direction(), exitCell, exitDir)) {
            score_.onGate();
            // Collision at the gate exit cell.
            if (board_.isWall(exitCell.y, exitCell.x)) return false;
            for (const Point& s : snake_.body()) {
                if (s == exitCell) return false;
            }
            int item = items_.consumeAt(board_, exitCell);
            bool grow = (item == CELL_GROWTH);
            snake_.teleport(exitCell, exitDir, grow);
            if (item == CELL_GROWTH) score_.onGrowth();
            if (item == CELL_POISON) { score_.onPoison(); snake_.shrink(); }
            score_.setLength(snake_.length());
            if (snake_.length() < kMinLength) return false;
            return true;
        }
        return false;  // no valid exit -> blocked -> game over
    }

    // --- Normal forward move -----------------------------------------
    // Wall collision.
    if (board_.isWall(nh.y, nh.x)) return false;

    // Body collision (the tail cell is about to vacate, so ignore it).
    const std::deque<Point>& b = snake_.body();
    for (size_t i = 0; i + 1 < b.size(); ++i) {
        if (b[i] == nh) return false;
    }

    int item = items_.consumeAt(board_, nh);
    if (item == CELL_GROWTH) {
        snake_.advance(nh, true);
        score_.onGrowth();
    } else if (item == CELL_POISON) {
        snake_.advance(nh, false);
        snake_.shrink();
        score_.onPoison();
    } else {
        snake_.advance(nh, false);
    }

    score_.setLength(snake_.length());
    if (snake_.length() < kMinLength) return false;
    return true;
}

void Game::drawSnake() const {
    const std::deque<Point>& b = snake_.body();
    for (size_t i = 0; i < b.size(); ++i) {
        if (i == 0) drawCell(b[i].y, b[i].x, 'O', CP_HEAD);
        else        drawCell(b[i].y, b[i].x, 'o', CP_BODY);
    }
}

void Game::render() const {
    erase();
    board_.draw();

    // Overlay items and gates from the grid.
    for (int y = 0; y < board_.height(); ++y) {
        for (int x = 0; x < board_.width(); ++x) {
            int c = board_.at(y, x);
            if (c == CELL_GROWTH) drawCell(y, x, '+', CP_GROWTH);
            else if (c == CELL_POISON) drawCell(y, x, '-', CP_POISON);
            else if (c == CELL_GATE)   drawCell(y, x, '@', CP_GATE);
        }
    }

    drawSnake();
    score_.draw(board_.width() * 2 + 3, stage_);

    if (paused_) {
        attron(COLOR_PAIR(CP_TEXT));
        mvprintw(board_.height() / 2, board_.width() - 3, " PAUSED ");
        attroff(COLOR_PAIR(CP_TEXT));
    }
    refresh();
}

bool Game::showResult(const char* title, const char* subtitle) {
    int cy = board_.height() / 2;
    int cx = board_.width();   // roughly centre in char columns

    attron(COLOR_PAIR(CP_TEXT));
    mvprintw(cy - 1, cx - 8, "==================");
    mvprintw(cy,     cx - 8, "  %s", title);
    mvprintw(cy + 1, cx - 8, "  %s", subtitle);
    mvprintw(cy + 2, cx - 8, "==================");
    attroff(COLOR_PAIR(CP_TEXT));
    refresh();

    // Discard any keystrokes buffered during play, then block for
    // one deliberate keypress. Any key continues; Q quits.
    flushinp();
    nodelay(stdscr, FALSE);
    bool again = false;
    while (true) {
        int ch = getch();
        if (ch == ERR) continue;
        again = !(ch == 'q' || ch == 'Q');
        break;
    }
    nodelay(stdscr, TRUE);
    return again;
}

void Game::run() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initCurses();
    running_ = true;
    startStage(0);

    using clock = std::chrono::steady_clock;
    auto last      = clock::now();
    auto stageBeg  = clock::now();

    while (running_) {
        handleInput();
        if (!running_) break;

        auto now = clock::now();

        if (!paused_) {
            long elapsedMs = static_cast<long>(
                std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count());

            if (elapsedMs >= kTickMs) {
                last = now;
                ++tick_;

                items_.update(board_, snake_, tick_);

                bool alive = step();

                int secs = static_cast<int>(
                    std::chrono::duration_cast<std::chrono::seconds>(now - stageBeg).count());
                score_.tickSeconds(secs);

                if (!alive) {
                    // Game over.
                    render();
                    bool again = showResult("GAME OVER", "R: retry  Q: quit");
                    if (again) { startStage(stage_); stageBeg = clock::now(); last = clock::now(); }
                    else running_ = false;
                    continue;
                }

                if (score_.missionComplete()) {
                    render();
                    if (stage_ + 1 < board_.stageCount()) {
                        showResult("STAGE CLEAR", "Press any key: next");
                        startStage(stage_ + 1);
                        stageBeg = clock::now();
                        last = clock::now();
                    } else {
                        bool again = showResult("YOU WIN!", "R: replay  Q: quit");
                        if (again) { startStage(0); stageBeg = clock::now(); last = clock::now(); }
                        else running_ = false;
                    }
                    continue;
                }
            }
        }

        render();
        napms(10);
    }

    endCurses();
}
