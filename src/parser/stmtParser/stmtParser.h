#pragma once
typedef struct ImportStatement ImportStatement;
typedef struct TopLevelStatement TopLevelStatement;
typedef struct Statement Statement;
typedef struct ProcedureCall ProcedureCall;
typedef struct StatementList StatementList;
typedef struct TopLevelStatementList TopLevelStatementList;

struct StatementList {
    int count;
    int capacity;
    Statement* data;
};

struct TopLevelStatementList {
    int count;
    int capacity;
    TopLevelStatement* data;
};

TopLevelStatement* parser_top_level_stmt();

void StatementList_push(StatementList* list, Statement* item);
void TopLevelStatementList_push(TopLevelStatementList* list, TopLevelStatement* item);
ProcedureCall* parser_procedure_call();
Statement* parser_statement();
