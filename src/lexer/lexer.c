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
static void lex_skipwhitespace(shLexer *lexer);
static shToken *lex_advancewithcurrent(shLexer *lexer, shTokenType type);
static shToken *lex_parseid(shLexer *lexer);
static shToken *lex_parseRunblock(shLexer *lexer);
static shToken *lex_parsestring(shLexer *lexer);
static void lex_parsecmt(shLexer *lexer);
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
        lex_skipwhitespace(lexer);

        if (isalpha(lexer->c))
        {
            shToken *id = lex_parseid(lexer);

            if (strncmp(id->value, "run", 3) == 0)
            {
                shToken_Free(id); // now we don need id
                return lex_parseRunblock(lexer);
            }

            return id;
        }

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
            lex_parsecmt(lexer);
            break;
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

static void lex_skipwhitespace(shLexer *lexer)
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

    int beginline = lexer->line;
    int begincolumn = lexer->column;

    while (isalnum(lexer->c) || lexer->c == '_')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';
        lex_advance(lexer);
    }

    int keyword_idx =
        shlr_utils_inStrArr(value, (char **)shlr_keywords, SH_KEYWORDS_COUNT);
    if (keyword_idx != -1)
    {
        shToken *tok =
            shToken_Init(SH_TOKEN_KEYWORD, value, beginline, begincolumn);

        tok->keyword = (shKeywordType)keyword_idx;

        return tok;
    }
    else
        return shToken_Init(SH_TOKEN_IDENTIFIER, value, beginline, begincolumn);
}

static shToken *lex_parseRunblock(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    int beginline = lexer->line;
    int begincolumn = lexer->column;

    while (lexer->c != '\0')
    {
        if (strncmp(&lexer->src[lexer->pos], "end", 3) == 0)
        {
            lex_advance(lexer); // e
            lex_advance(lexer); // n
            lex_advance(lexer); // d

            return shToken_Init(SH_TOKEN_RUNBLOCK, value, beginline,
                                begincolumn);
        }

        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';

        lex_advance(lexer);
    }

    shlr_logger_fatal(1, "missing end of run block at line %d, column %d\n",
                      beginline, begincolumn);

    return NULL; // unreachable but my lsp dosen't get it
}

static shToken *lex_parsestring(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    lex_advance(lexer); // skip the first "

    int beginline = lexer->line;
    int begincolumn = lexer->column;

    while (lexer->c != '"')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';

        lex_advance(lexer);
    }

    lex_advance(lexer); // skip the last "

    return shToken_Init(SH_TOKEN_STRING, value, beginline, begincolumn);
}

// it will just skip the funciton and all it does is just advance the lexer
static void lex_parsecmt(shLexer *lexer)
{
    char *value = malloc(sizeof(char));

    lex_advance(lexer); // skip the #

    int beginline = lexer->line;
    int begincolumn = lexer->column;

    while (lexer->c != '\n')
    {
        value = realloc(value, sizeof(char) * (strlen(value) + 2));
        value[strlen(value)] = lexer->c;
        value[strlen(value) + 1] = '\0';

        lex_advance(lexer);
    }

    lex_advance(lexer); // skip the \n
}
