CC = g++

CSTD = c++20

CFLAGS = -std=$(CSTD) -Werror -Wall -Wextra -Wpedantic -Wuninitialized
CFLAGS += -Wmissing-include-dirs -Wshadow -Wundef -Winvalid-pch
CFLAGS += -Wzero-as-null-pointer-constant -Wctor-dtor-privacy -Wold-style-cast
CFLAGS += -Woverloaded-virtual

AUTOFMT = clang-format
STYLE = GNU

SRC = src
INCL = include
SRC_FILES = $(wildcard $(SRC)/*.cpp)
INCL_FILES = $(wildcard $(INCL)/*.hpp)
OBJ_FILES = $(SRC_FILES:$(SRC)/%.cpp=$(SRC)/%.o)

LIBS =

TARGET = emulator

all: clean format $(TARGET)
full: all test

$(SRC)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INCL)/ $(LIBS)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) -I./$(INCL)/ $(LIBS)

format:
	$(AUTOFMT) -style=$(STYLE) -i $(SRC_FILES) $(INCL_FILES)

test: $(TARGET)
	valgrind --leak-check=yes ./$(TARGET)

clean:
	-@rm vgcore.* 2>/dev/null || true
	-@rm $(OBJ_FILES) 2>/dev/null || true
	-@rm $(TARGET) 2>/dev/null || true
