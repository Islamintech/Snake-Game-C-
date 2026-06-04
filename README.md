# Snake Game (C++ / curses)

A classic Snake game written in C++ using the curses terminal UI, built for a
C++ class term project. It is fully object-oriented — each component is its own
class split into separate header (`.h`) and implementation (`.cpp`) files — and
implements all five project stages: map, snake, items, gates, and the
scoreboard/mission system.

## Features

- 21×21 board with two wall types (normal wall + immune corner wall)
- Snake movement with tick-based timing and non-blocking input
- Growth (`+`) and poison (`-`) items that spawn and expire over time
- Gate pairs with directional exit logic (edge vs. interior walls)
- Live scoreboard: current/max length (B), growth (+), poison (-), gates (G), time
- Per-stage missions and progression across **4 distinct stage maps**
- Game over / stage clear / win screens

## Requirements

- **Compiler:** MinGW-w64 g++ (tested with 15.2.0), C++11
- **Make:** `mingw32-make` (GNU Make)
- **Library:** PDCurses (bundled in `pdcurses/`, prebuilt `pdcurses/wincon/pdcurses.a`)

> The code uses only the standard curses API, so on Linux it can be built against
> `ncurses` by adjusting the include/link flags in the `Makefile`.

## Build & Run

```sh
mingw32-make        # produces snake.exe
snake.exe           # play

mingw32-make clean  # remove build artifacts
```

> On Windows, close the running game before rebuilding — otherwise the linker
> cannot overwrite the locked `snake.exe`.

## Controls

| Key | Action |
|-----|--------|
| Arrow keys / W A S D | Move the snake |
| P | Pause / resume |
| Q | Quit |
| Any key | Continue from a game over / stage clear screen |

## Rules

- The snake starts at length 3 and moves automatically each tick.
- **Game over** when the snake hits a wall, hits itself, reverses into its own
  neck, or shrinks below length 3.
- Growth item: length +1. Poison item: length −1.
- Entering one gate teleports the head out of its paired gate.
- Clear a stage by completing all of its missions (B / + / − / G goals).

## Project Structure

| File | Class | Responsibility |
|------|-------|----------------|
| `common.h` | — | Cell codes, `Dir`, `Point`, direction helpers |
| `board.h/.cpp` | `Board` | Map grid, stage layouts, rendering |
| `snake.h/.cpp` | `Snake` | Body, movement, grow/shrink, gate teleport |
| `item.h/.cpp` | `Item`, `ItemManager` | Growth/poison item lifecycle |
| `gate.h/.cpp` | `Gate` | Gate pair placement and exit logic |
| `scoreboard.h/.cpp` | `ScoreBoard` | Stats, missions, side panel |
| `game.h/.cpp` | `Game` | Main loop, input, collisions, stage flow |
| `main.cpp` | — | Entry point |
| `Makefile` | — | Build script |
| `결과보고서.md` | — | Project report (Korean) |
