#ifndef SHLR_LEXER_LEXER_H
#define SHLR_LEXER_LEXER_H

#include "token.h"
#include <stddef.h>

typedef struct _shLexer
{
    const char *src;
    size_t src_len;
    char c;
    int line;
    int column;
    int pos;
} shLexer;

shLexer *shLexer_Init(const char *src, size_t src_len);
shToken *shLexer_Next(shLexer *lexer);
void shLexer_Free(shLexer *lexer);

#endif
