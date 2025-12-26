CC := gcc
CFLAGS := -Wall -Wextra -std=c11

RUNFLAGS := examples/main.vty -v

SRC := src/main.c \
	src/lexer/lexer.c

BIN := bin/verity

TEST_SRCS := $(wildcard tests/unit/test_*.c)
TEST_BIN := $(TEST_SRCS:tests/unit/%.c=bin/%)

.PHONY: all clean run

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	@-figlet -c Verity
	$(CC) $(CFLAGS) -o $@ $^

bin/%: tests/unit/%.c src/lexer/lexer.c
	@- figlet -c Verity
	@- echo Compiling tests for the Verity Compiler:
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN) $(RUNFLAGS)

clean:
	@rm -f $(BIN)

test: $(TEST_BIN)
	@- echo Running tests for the Verity Compiler:
	@python tests/tools/run.py