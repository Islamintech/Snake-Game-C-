# ============================================================
#  Makefile - Snake Game term project
#
#  Build (Windows / MinGW):   mingw32-make
#  Run:                       snake.exe
#  Clean:                     mingw32-make clean
#
#  Layout:
#    src/                game source (.h/.cpp)
#    third_party/pdcurses  vendored PDCurses library
#    build/              object files (generated)
#    snake.exe           final executable (generated)
#
#  Links against the vendored PDCurses static library.
#  On Linux, override with: make CURSES_INC= CURSES_LIB="-lncurses"
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -O2

SRC_DIR   := src
BUILD_DIR := build

# --- PDCurses (Windows) ---
CURSES_INC := -Ithird_party/pdcurses
CURSES_LIB := third_party/pdcurses/wincon/pdcurses.a

# Windows console libraries required by PDCurses (wincon port).
SYS_LIB  := -lwinmm

TARGET   := snake.exe
SRCS     := main.cpp game.cpp board.cpp snake.cpp item.cpp gate.cpp scoreboard.cpp
OBJS     := $(addprefix $(BUILD_DIR)/,$(SRCS:.cpp=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(CURSES_LIB) $(SYS_LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CURSES_INC) -c $< -o $@

$(BUILD_DIR):
	-mkdir -p $(BUILD_DIR)

# --- Dependencies ---
$(BUILD_DIR)/main.o:       $(SRC_DIR)/game.h
$(BUILD_DIR)/game.o:       $(SRC_DIR)/game.h $(SRC_DIR)/board.h $(SRC_DIR)/snake.h $(SRC_DIR)/item.h $(SRC_DIR)/gate.h $(SRC_DIR)/scoreboard.h $(SRC_DIR)/common.h
$(BUILD_DIR)/board.o:      $(SRC_DIR)/board.h $(SRC_DIR)/common.h
$(BUILD_DIR)/snake.o:      $(SRC_DIR)/snake.h $(SRC_DIR)/common.h
$(BUILD_DIR)/item.o:       $(SRC_DIR)/item.h $(SRC_DIR)/board.h $(SRC_DIR)/snake.h $(SRC_DIR)/common.h
$(BUILD_DIR)/gate.o:       $(SRC_DIR)/gate.h $(SRC_DIR)/board.h $(SRC_DIR)/common.h
$(BUILD_DIR)/scoreboard.o: $(SRC_DIR)/scoreboard.h $(SRC_DIR)/common.h

clean:
	-rm -f $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
