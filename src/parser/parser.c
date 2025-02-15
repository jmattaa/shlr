#include "parser/parser.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "logger.h"
#include "parser/AST.h"
#include <stdlib.h>

// ---------------------- HELPER FUNC DELC -----------------------------------
static shToken *parser_next(shParser *parser);
static shToken *parser_expect(shParser *parser, shTokenType type);
static shAstNode *parser_parseKeyword(shParser *parser);
static shAstNode *parser_parseVar(shParser *parser);
static shAstNode *parser_parseTarg(shParser *parser);
// ---------------------------------------------------------------------------

shParser *shParser_Init(shLexer *lexer)
{
    shParser *parser = malloc(sizeof(shParser));

    parser->lexer = lexer;
    parser->current_token = shLexer_Next(parser->lexer);

    return parser;
}

shAstNode *shParser_Parse(shParser *parser)
{
    shAstNode *root = shAstNode_Init(SH_AST_ROOT, NULL, NULL);

    while (parser->current_token->type != SH_TOKEN_EOF)
    {
        switch (parser->current_token->type)
        {
        case SH_TOKEN_KEYWORD:
            shAstNode_AddChild(root, parser_parseKeyword(parser));
            break;
        default:
            shlr_logger_fatal(
                1, "unexpected '%s' at line %d, column %d",
                shlr_token_type_names[parser->current_token->type],
                parser->current_token->line, parser->current_token->column);
            break;
        }
    }

    return root;
}

void shParser_Free(shParser *parser)
{
    shToken_Free(parser->current_token);
    free(parser);
}

// ---------------------- HELPER FUNC IMPL -----------------------------------
static shToken *parser_next(shParser *parser)
{
    shToken *token = parser->current_token;
    parser->current_token = shLexer_Next(parser->lexer);
    return token;
}

static shToken *parser_expect(shParser *parser, shTokenType type)
{
    if (parser->current_token->type != type)
        shlr_logger_fatal(1, "expected '%s' at line %d, column %d but got '%s'",
                          shlr_token_type_names[type],
                          parser->current_token->line,
                          parser->current_token->column,
                          shlr_token_type_names[parser->current_token->type]);

    return parser_next(parser);
}

static shAstNode *parser_parseKeyword(shParser *parser)
{
    shToken *keyword = parser_expect(parser, SH_TOKEN_KEYWORD);

    shAstNode *node = shAstNode_Init(-1, NULL, NULL);

    switch (keyword->keyword)
    {
    case SH_KEYWORD_var:
        node = parser_parseVar(parser);
        break;
    case SH_KEYWORD_targ:
        node = parser_parseTarg(parser);
        break;
    default:
        shlr_logger_fatal(1, "unexpected '%s' at line %d, column %d",
                          keyword->value, keyword->line, keyword->column);
    }

    return node;
}

static shAstNode *parser_parseVar(shParser *parser)
{
    shToken *name = parser_expect(parser, SH_TOKEN_IDENTIFIER);
    shToken *val =
        parser_expect(parser, SH_TOKEN_STRING); // all variables are strings

    shAstNode *node = shAstNode_Init(SH_AST_VAR, name->value, val->value);

    return node;
}

static shAstNode *parser_parseTarg(shParser *parser)
{
    shToken *name = parser_expect(parser, SH_TOKEN_IDENTIFIER);

    shAstNode *node = shAstNode_Init(SH_AST_TARG, name->value, NULL);

    // handle depend if exists
    if (parser->current_token->keyword == SH_KEYWORD_depends)
    {
        parser_expect(parser, SH_TOKEN_KEYWORD);
        parser_expect(parser, SH_TOKEN_LBRACKET);

        while (parser->current_token->type == SH_TOKEN_IDENTIFIER)
        {
            shToken *dep = parser_expect(parser, SH_TOKEN_IDENTIFIER);
            shAstNode_AddChild(node,
                               shAstNode_Init(SH_AST_DEPEND, dep->value, NULL));

            if (parser->current_token->type == SH_TOKEN_RBRACKET)
                break;
        }

        parser_expect(parser, SH_TOKEN_RBRACKET);
    }

    // there must be a run in each targ
    shToken *run = parser_next(parser);
    if (run->type != SH_TOKEN_RUNBLOCK)
        shlr_logger_fatal(1, "expected 'run' at line %d, column %d", run->line,
                          run->column);

    // the end of target
    if (parser->current_token->keyword != SH_KEYWORD_end)
        shlr_logger_fatal(1, "expected 'end' at line %d, column %d",
                          parser->current_token->line,
                          parser->current_token->column);
    parser_next(parser);

    shAstNode_AddChild(node, shAstNode_Init(SH_AST_RUNBLOCK, NULL, run->value));

    return node;
}
