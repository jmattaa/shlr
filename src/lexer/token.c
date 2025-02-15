#include "lexer/token.h"
#include <stdlib.h>

#define _X(x) #x,
const char *shlr_keywords[SH_KEYWORDS_COUNT] = {SH_KEYWORDS_ITER(_X)};
#undef _X

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

#define _X(x) #x,
const char *shlr_token_type_names[SH_TOKEN_TYPES_COUNT] = {
    SH_TOKEN_TYPES_ITER(_X)};
#undef _X
