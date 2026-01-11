#include "./semantic.h"

void semantics_analyze(Program ast){
    for(int i = 0; i < ast.list->count; i ++){
        printf("top-level %d/%d\n", i + 1, ast.list->count);
        ast.list->data[i * sizeof(TopLevelStatement)];
    }
}
