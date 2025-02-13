#include "lexer/token.h"
#include <stdlib.h>

const char *shlr_keywords[SH_KEYWORDS_COUNT] = {"var", "targ", "depends", "run",
                                                "end"};

shToken *shToken_Init(shTokenType type, char *value, int line, int column)
{
    shToken *token = malloc(sizeof(shToken));
    if (token == NULL)
        return NULL;

    token->type = type;
    token->value = value;
    token->line = line;
    token->column = column;

    return token;
}

void shToken_Free(shToken *token)
{
    free(token->value);
    free(token);
}
