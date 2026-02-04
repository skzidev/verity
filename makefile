CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -g

RUNFLAGS := examples/main.vty

SRC := $(shell find src -name '*.c')

BIN := bin/verity

TEST_SRCS := $(wildcard tests/unit/test_*.c)
TEST_BIN := $(TEST_SRCS:tests/unit/%.c=bin/%)

.PHONY: all clean run

all: $(BIN) $(TEST_BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/%: tests/unit/%.c src/lexer/lexer.c src/diags/diagnostics.c src/config/config.c
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN) ## Build and run the compiler
	@echo " ============= program output ============="
	./$(BIN) $(RUNFLAGS)

clean: ## Remove build artifacts
	@rm -f $(BIN)

test: $(TEST_BIN) ## Run the testing suite
	@python tests/tools/run.py

lint: ## Run static code analysis
	@echo "======= This command requires cppcheck and clang-tidy to be installed. ======="
	cppcheck $(SRC) $(TEST_SRCS)
	clang-tidy $(SRC) $(TEST_SRCS) --fix-notes

todo: ## List todos across the project
	@python tests/tools/todos.py

.PHONY: help
help: ## Show this help
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\t\033[36m%-20s\033[0m %s\n", $$1, $$2}'
