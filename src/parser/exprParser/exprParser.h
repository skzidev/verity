typedef struct Expression Expression;
typedef struct PrimaryExpression PrimaryExpression;
typedef struct UnaryExpression UnaryExpression;
typedef struct MulExpression MulExpression;

Expression* parser_expression();
PrimaryExpression* parser_primary_expression();
typedef struct ExpressionList ExpressionList;
ExpressionList* parser_expression_list();
