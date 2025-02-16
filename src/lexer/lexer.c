#include "lexer/lexer.h"
#include "lexer/token.h"
#include "logger.h"
#include "utils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//  ----------------- HELPER FUNCTIONS DEFINITIONS ---------------------------
static void lex_advance(shLexer *lexer);
static void lex_skipwhitespace(shLexer *lexer);
static shToken *lex_advancewithcurrent(shLexer *lexer, shTokenType type);
static shToken *lex_parseid(shLexer *lexer);
static shToken *lex_parseRunblock(shLexer *lexer);
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
        lex_skipwhitespace(lexer);

        if (lexer->c == '#' || lexer->line == lexer->last_useful_cmt_line)
            return lex_parsecmt(lexer);

        lex_advance(lexer);
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

static shToken *lex_parsecmt(shLexer *lexer)
{
    lex_advance(lexer); // skip #
    lex_skipwhitespace(lexer);

    if (isalnum(lexer->c) || lexer->c == '_')
    {
        lexer->last_useful_cmt_line = lexer->line;
        return lex_parseid(lexer);
    }

    while (lexer->c != '\n' && lexer->c != '\0')
        lex_advance(lexer);

    return shToken_Init(SH_TOKEN_COMMENT, NULL, lexer->line, lexer->column);
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

    return shToken_Init(SH_TOKEN_IDENTIFIER, value, beginline, begincolumn);
}
