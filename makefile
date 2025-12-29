CC := gcc
CFLAGS := -Wall -Wextra -std=c11

RUNFLAGS := examples/main.vty

SRC := src/main.c \
	src/lexer/lexer.c

BIN := bin/verity

TEST_SRCS := $(wildcard tests/unit/test_*.c)
TEST_BIN := $(TEST_SRCS:tests/unit/%.c=bin/%)

.PHONY: all clean run

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/%: tests/unit/%.c src/lexer/lexer.c
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN) $(RUNFLAGS)

clean:
	@rm -f $(BIN)

test: $(TEST_BIN)
	@python tests/tools/run.py