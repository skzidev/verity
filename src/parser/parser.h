#include "../lexer/lexer.h"

typedef struct {
	size_t pos;
	TokenArray tokens;
} Parser;