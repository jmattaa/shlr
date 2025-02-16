#include "shlr.h"
#include "logger.h"
#include "parser/AST.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 256

// append value to script
void append_to_script(char **script, size_t *script_length, size_t *buffer_size, const char *value)
{
    size_t value_length = strlen(value);

    while (*script_length + value_length + 1 > *buffer_size)
    {
        *buffer_size *= 2;
        *script = realloc(*script, *buffer_size);
        if (!*script)
            shlr_logger_fatal(1, "memory allocation for script failed! :(");
    }

    strcat(*script, value);
    *script_length += value_length;
}

// get the runblock of the target
void process_target_commands(shAstNode *node, char **script, size_t *script_length, size_t *buffer_size)
{
    for (int i = 0; i < node->children_count; i++)
    {
        shAstNode *child = node->children[i];

        if (child->type == SH_AST_RUNBLOCK)
        {
            append_to_script(script, script_length, buffer_size, child->value);
            break; // there will only be one runblock
        }
    }
}

char *shlr_createTargScript(shAstNode *root, char *targ)
{
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t script_length = 0;
    char *script = malloc(buffer_size);
    if (!script)
        shlr_logger_fatal(1, "memory allocation for script failed! :(");

    script[0] = '\0';

    // initial runblock
    if (root->children[0]->type == SH_AST_RUNBLOCK)
    {
        append_to_script(&script, &script_length, &buffer_size, root->children[0]->value);
        append_to_script(&script, &script_length, &buffer_size, "\n");
    }

    // if we don't have a target just take the first one
    if (targ == NULL)
    {
        for (int i = 0; i < root->children_count; i++)
        {
            if (root->children[i]->type == SH_AST_TARG)
            {
                process_target_commands(root->children[i], &script, &script_length, &buffer_size);
                return script;
            }
        }
        return script;
    }

    // find and process the requested target
    int targ_found = 0;
    for (int i = 0; i < root->children_count; i++)
    {
        shAstNode *child = root->children[i];
        if (child->type == SH_AST_TARG && child->name && strcmp(child->name, targ) == 0)
        {
            process_target_commands(child, &script, &script_length, &buffer_size);
            targ_found = 1;
            break;
        }
    }

    if (!targ_found)
        shlr_logger_fatal(1, "target '%s' not found!", targ);

    return script;
}

