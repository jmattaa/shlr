#ifndef SHLR_PARSER_H
#define SHLR_PARSER_H

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/AST.h"

typedef struct _shParser
{
    shLexer *lexer;
    shToken *current_token;
} shParser;

shParser *shParser_Init(shLexer *lexer);
shAstNode *shParser_Parse(shParser *parser);
void shParser_Free(shParser *parser);

#endif
