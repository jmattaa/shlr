#include "parser/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

shAstNode *shAstNode_Init(shAstType type, char *name, char *value)
{
    shAstNode *node = (shAstNode *)malloc(sizeof(shAstNode));
    if (!node)
        return NULL;

    node->type = type;
    node->name = name ? strdup(name) : NULL;
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->children_count = 0;

    return node;
}

void shAstNode_AddChild(shAstNode *parent, shAstNode *child)
{
    parent->children = realloc(
        parent->children, sizeof(shAstNode *) * (parent->children_count + 1));
    parent->children[parent->children_count] = child;
    parent->children_count++;
}

void shAstNode_Free(shAstNode *node)
{
    if (!node)
        return;

    free(node->name);
    free(node->value);

    for (int i = 0; i < node->children_count; i++)
        shAstNode_Free(node->children[i]);

    free(node->children);
    free(node);
}

void shAstNode_Print(shAstNode *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");
    printf("Node Type: %d, Name: %s, Value: %s\n", node->type, node->name,
           node->value ? node->value : "NULL");

    for (int i = 0; i < node->children_count; i++)
        shAstNode_Print(node->children[i], indent + 1);
}
