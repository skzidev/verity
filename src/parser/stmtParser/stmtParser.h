typedef struct ImportStatement ImportStatement;
typedef struct TopLevelStatement TopLevelStatement;
typedef struct Statement Statement;

TopLevelStatement* parser_top_level_stmt();

Statement* parser_statement();
