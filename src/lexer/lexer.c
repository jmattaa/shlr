#include "lexer/lexer.h"
#include "lexer/token.h"
#include "logger.h"
#include "utils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// HELPER FUNCTIONS DEFINITIONS
static void lex_advance(shLexer *lexer);
static void lex_shipwhitespace(shLexer *lexer);
static shToken *lex_advancewithcurrent(shLexer *lexer, shTokenType type);
static shToken *lex_parseid(shLexer *lexer);
static shToken *lex_parsestring(shLexer *lexer);
static shToken *lex_parsecmt(shLexer *lexer);
// ---------------------------------------------------------------------------

shLexer *shLexer_Init(const char *src, size_t src_len)
{
    shLexer *lexer = malloc(sizeof(shLexer));
    if (!lexer)
        return NULL;

    lexer->src = src;
    lexer->src_len = src_len;
    lexer->c = src[0];
    lexer->line = 1;
    lexer->column = 1;
    lexer->pos = 0;

    return lexer;
}

shToken *shLexer_Next(shLexer *lexer)
{
    while (lexer->c != '\0')
    {
        lex_shipwhitespace(lexer);

        if (isalpha(lexer->c))
            return lex_parseid(lexer);

        switch (lexer->c)
        {
        case '\0':
            break;
        case '"':
            return lex_parsestring(lexer);
        case '[':
            return lex_advancewithcurrent(lexer, SH_TOKEN_LBRACKET);
        case ']':
            return lex_advancewithcurrent(lexer, SH_TOKEN_RBRACKET);
        case '#':
            return lex_parsecmt(lexer);
        default:
            shlr_logger_fatal(1,
                              "Unknown character '%c' at line %d, column %d\n",
                              lexer->c, lexer->line, lexer->column);
        }
    }

    return shToken_Init(SH_TOKEN_EOF, NULL, lexer->line, lexer->column);
}

void shLexer_Free(shLexer *lexer) { free(lexer); }

// -------------------- HELPER FUNCTION IMPLEMENTATIONS ------------------------
static void lex_advance(shLexer *lexer)
{
    if (lexer->pos > lexer->src_len)
        shlr_logger_fatal(1, "if you see this, then I've fucked up big time, "
                             "inform the developer, how did we even get here?");

    // update line and column
    if (lexer->c == '\n')
    {
        lexer->line++;
        lexer->column = 1;
    }
    else
    {
        lexer->column++;
    }

    lexer->pos++;
    lexer->c = lexer->src[lexer->pos];
}

static void lex_shipwhitespace(shLexer *lexer)
{
    while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
    {
        lex_advance(lexer);
    }
}

static shToken *lex_advancewithcurrent(shLexer *lexer, shTokenType type)
{
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';

    shToken *token = shToken_Init(type, value, lexer->line, lexer->column);

    lex_advance(lexer);
    return token;
}

static shToken *lex_parseid(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    while (isalnum(lexer->c) || lexer->c == '_')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';
        lex_advance(lexer);
    }

    if (shlr_utils_inStrArr(value, (char **)shlr_keywords, SH_KEYWORDS_COUNT))
        return shToken_Init(SH_TOKEN_KEYWORD, value, lexer->line,
                            lexer->column);
    else
        return shToken_Init(SH_TOKEN_IDENTIFIER, value, lexer->line,
                            lexer->column);
}

static shToken *lex_parsestring(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    lex_advance(lexer); // skip the first "

    while (lexer->c != '"')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';

        lex_advance(lexer);
    }

    lex_advance(lexer); // skip the last "

    return shToken_Init(SH_TOKEN_STRING, value, lexer->line, lexer->column);
}

static shToken *lex_parsecmt(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    lex_advance(lexer); // skip the #

    while (lexer->c != '\n')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';

        lex_advance(lexer);
    }

    lex_advance(lexer); // skip the \n

    return shToken_Init(SH_TOKEN_COMMENT, value, lexer->line, lexer->column);
}
