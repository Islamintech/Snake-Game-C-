# ============================================================
#  Makefile - Snake Game term project
#
#  Build (Windows / MinGW):   mingw32-make
#  Run:                       snake.exe
#  Clean:                     mingw32-make clean
#
#  Links against the vendored PDCurses static library.
#  On Linux, override with: make CURSES_INC= CURSES_LIB="-lncurses"
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2

# --- PDCurses (Windows) ---
CURSES_INC := -Ipdcurses
CURSES_LIB := pdcurses/wincon/pdcurses.a

# Windows console libraries required by PDCurses (wincon port).
SYS_LIB  := -lwinmm

TARGET   := snake.exe
OBJS     := main.o game.o board.o snake.o item.o gate.o scoreboard.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(CURSES_LIB) $(SYS_LIB)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CURSES_INC) -c $< -o $@

# --- Dependencies ---
main.o:       main.cpp game.h
game.o:       game.cpp game.h board.h snake.h item.h gate.h scoreboard.h common.h
board.o:      board.cpp board.h common.h
snake.o:      snake.cpp snake.h common.h
item.o:       item.cpp item.h board.h snake.h common.h
gate.o:       gate.cpp gate.h board.h common.h
scoreboard.o: scoreboard.cpp scoreboard.h common.h

clean:
	-del /Q $(OBJS) $(TARGET) 2>nul

.PHONY: all clean
