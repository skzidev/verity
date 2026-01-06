typedef struct ImportStatement ImportStatement;
typedef struct TopLevelStatement TopLevelStatement;
typedef struct Statement Statement;
typedef struct ProcedureCall ProcedureCall;

TopLevelStatement* parser_top_level_stmt();

ProcedureCall* parser_procedure_call();
Statement* parser_statement();
