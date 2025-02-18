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

void shAstNode_ListTargs(shAstNode *node, shlrCli *cli)
{
#define COLOR_RESET "\033[0m"
#define COLOR_DEPEND "\033[1;32m"
#define COLOR_DIVIDER "\033[1;33m"
#define COLOR_NO_DEPEND "\033[1;31m"

    for (int i = 0; i < node->children_count; i++)
    {
        if (node->children[i]->type == SH_AST_TARG)
        {
            if (cli->listdeps)
                printf(COLOR_DIVIDER "----------\n" COLOR_RESET);

            printf("%s\n", node->children[i]->name);

            if (cli->listdeps)
            {
                printf(COLOR_DIVIDER "Depends on:\n" COLOR_RESET);
                int has_dependencies = 0;
                for (int j = 0; j < node->children[i]->children_count; j++)
                {
                    if (node->children[i]->children[j]->type == SH_AST_DEPEND)
                    {
                        printf("  - %s\n",
                               node->children[i]->children[j]->name);
                        has_dependencies = 1;
                    }
                }

                if (!has_dependencies)
                    printf(COLOR_NO_DEPEND "  (No dependencies)\n" COLOR_RESET);

                printf("\n");
            }
        }
    }
}
