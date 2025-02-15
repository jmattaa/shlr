#ifndef SHLR_PARSER_AST_H
#define SHLR_PARSER_AST_H

typedef enum _shAstType
{
    SH_AST_ROOT, // will be the whole program
    SH_AST_VAR,
    SH_AST_TARG,
    SH_AST_DEPEND,
    SH_AST_RUNBLOCK
} shAstType;

typedef struct _shAstNode
{
    shAstType type;

    // targ = target name, var = var name
    char *name;
    char *value; // ONLY IF TYPE IS VAR OR RUNBLOCK
    struct _shAstNode **children;
    int children_count;
} shAstNode;

shAstNode *shAstNode_Init(shAstType type, char *name, char *value);
void shAstNode_AddChild(shAstNode *parent, shAstNode *child);
void shAstNode_Free(shAstNode *node);

void shAstNode_Print(shAstNode *node, int indent);

#endif
