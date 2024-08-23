CC = g++

CSTD = c++20

CFLAGS = -std=$(CSTD) -Wall -Wextra -Wpedantic -Wuninitialized
CFLAGS += -Wmissing-include-dirs -Wshadow -Wundef -Winvalid-pch
CFLAGS += -Wzero-as-null-pointer-constant -Wctor-dtor-privacy -Wold-style-cast
CFLAGS += -Woverloaded-virtual

AUTOFMT = clang-format
STYLE = GNU

SRC = src
INCL = include
SRC_FILES = $(wildcard $(SRC)/*.cpp)
INCL_FILES = $(wildcard $(INCL)/*.h)
OBJ_FILES = $(SRC_FILES:$(SRC)/%.c=$(SRC)/%.o)

LIBS =

TARGET = emulator

all: clean format $(TARGET)
full: all test

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INCL_DIR)/ $(LIBS)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) -I./$(INCL)/ $(LIBS)

format:
	$(AUTOFMT) -style=$(STYLE) -i $(SRC_FILES) $(INCL_FILES)

test: $(TARGET)
	valgrind --leak-check=yes ./$(TARGET)

clean:
	-@rm $(SRC)/*.o 2>/dev/null || true
	-@rm $(TARGET) 2>/dev/null || true
