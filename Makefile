CC = g++

CSTD = C++20

CFLAGS = -Wall -Wextra -Wpedantic -Wuninitialized -Wmissing-include-dirs
CFLAGS += -Wshadow -Wundef -Winvalid-pch -Wzero-as-null-pointer-constant
CFLAGS += -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual

AUTOFMT = clang-format
STYLE = GNU

SRC = src
INCL = include
SRC_FILES = $(wildcard $(SRC)/*.cpp)
INCL_FILES = $(wildcard $(INCL)/*.h)
OBJ_FILES = $(SRC_FILES:$(SRC)/%.c=$(SRC)/%.o)

TARGET = emulator

all: clean format $(TARGET)
full: all test

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INCL_DIR)/

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) -I./$(INCL)/

format:
	$(AUTOFMT) -style=$(STYLE) -i $(SRC_FILES) $(INCL_FILES)

test: $(TARGET)
	valgrind --leak-check=yes ./$(TARGET)

clean:
	-@rm $(SRC)/*.o 2>/dev/null || true
	-@rm $(TARGET) 2>/dev/null || true
