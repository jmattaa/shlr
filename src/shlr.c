#include "shlr.h"
#include "logger.h"
#include "parser/AST.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 256
#define MAX_DEP_STACK_SIZE 128

// ------------------ HELPER FUNCTION DEFINITIONS ---------------------------
// append value to script
static void append_to_script(char **script, size_t *script_length,
                             size_t *buffer_size, const char *value);
// get the runblock of the target
static void process_target_commands(shAstNode *node, char **script,
                                    size_t *script_length, size_t *buffer_size);
// recursively process target and its dependencies
static void process_target_dependencies(shAstNode *root, shAstNode *target,
                                        char **script, size_t *script_length,
                                        size_t *buffer_size,
                                        char **dependency_stack,
                                        int *stack_size);
// ---------------------------------------------------------------------------

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
        append_to_script(&script, &script_length, &buffer_size,
                         root->children[0]->value);
        append_to_script(&script, &script_length, &buffer_size, "\n");
    }

    // Stack to track dependencies being processed
    char *dependency_stack[MAX_DEP_STACK_SIZE];
    int stack_size = 0;

    // if we don't have a target, just take the first one
    if (targ == NULL)
    {
        for (int i = 0; i < root->children_count; i++)
        {
            if (root->children[i]->type == SH_AST_TARG)
            {
                process_target_dependencies(root, root->children[i], &script,
                                            &script_length, &buffer_size,
                                            dependency_stack, &stack_size);
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
        if (child->type == SH_AST_TARG && child->name &&
            strcmp(child->name, targ) == 0)
        {
            process_target_dependencies(root, child, &script, &script_length,
                                        &buffer_size, dependency_stack,
                                        &stack_size);
            targ_found = 1;
            break;
        }
    }

    if (!targ_found)
        shlr_logger_fatal(1, "target '%s' not found!", targ);

    return script;
}

void shlr_runScript(char *script)
{
    system(script);
}

// ------------------------ HELPER FUNCTIONS DEFINITIONS ---------------------
static void append_to_script(char **script, size_t *script_length,
                             size_t *buffer_size, const char *value)
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

static void process_target_commands(shAstNode *node, char **script,
                                    size_t *script_length, size_t *buffer_size)
{
    for (int i = 0; i < node->children_count; i++)
    {
        shAstNode *child = node->children[i];

        if (child->type == SH_AST_RUNBLOCK)
        {
            append_to_script(script, script_length, buffer_size, child->value);
            append_to_script(script, script_length, buffer_size, "\n");
            break; // only one runblock
        }
    }
}

static void process_target_dependencies(shAstNode *root, shAstNode *target,
                                        char **script, size_t *script_length,
                                        size_t *buffer_size,
                                        char **dependency_stack,
                                        int *stack_size)
{
    for (int i = 0; i < *stack_size; i++)
    {
        if (strcmp(dependency_stack[i], target->name) == 0)
        {
            shlr_logger_fatal(1, "there is a circular dependency! :( at '%s'",
                              target->name);
        }
    }

    // Add current target to the stack
    if (*stack_size >= MAX_DEP_STACK_SIZE)
    {
        shlr_logger_fatal(
            1, "Dependency stack overflow! Too many nested dependencies.");
    }
    dependency_stack[*stack_size] = target->name;
    (*stack_size)++;

    for (int i = 0; i < target->children_count; i++)
    {
        shAstNode *child = target->children[i];

        if (child->type == SH_AST_DEPEND && child->name)
        {
            for (int j = 0; j < root->children_count; j++)
            {
                shAstNode *potential_target = root->children[j];
                if (potential_target->type == SH_AST_TARG &&
                    potential_target->name &&
                    strcmp(potential_target->name, child->name) == 0)
                {
                    process_target_dependencies(root, potential_target, script,
                                                script_length, buffer_size,
                                                dependency_stack, stack_size);
                    break;
                }
            }
        }
    }

    // place the runblock of current target
    process_target_commands(target, script, script_length, buffer_size);

    (*stack_size)--;
}
// ---------------------------------------------------------------------------
