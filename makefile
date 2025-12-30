CC := gcc
CFLAGS := -Wall -Wextra -std=c11

RUNFLAGS := examples/main.vty

SRC := $(wildcard src/*.c src/*/*.c)

BIN := bin/verity

TEST_SRCS := $(wildcard tests/unit/test_*.c)
TEST_BIN := $(TEST_SRCS:tests/unit/%.c=bin/%)

.PHONY: all clean run

all: $(BIN) $(TEST_BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	cppcheck $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

bin/%: tests/unit/%.c src/lexer/lexer.c src/diags/diagnostics.c
	cppcheck $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN) $(RUNFLAGS)

clean:
	@rm -f $(BIN)

test: $(TEST_BIN)
	@python tests/tools/run.py

lint: $(BIN)
	@echo This command requires cppcheck to be installed.
	cppcheck $(SRC)