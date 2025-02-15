#ifndef SHLR_LEXER_TOKEN_H
#define SHLR_LEXER_TOKEN_H

#define SH_TOKEN_TYPES_COUNT 8
#define SH_TOKEN_TYPES_ITER(_X)                                                \
    _X(SH_TOKEN_KEYWORD)                                                       \
    _X(SH_TOKEN_STRING)                                                        \
    _X(SH_TOKEN_IDENTIFIER)                                                    \
    _X(SH_TOKEN_LBRACKET)                                                      \
    _X(SH_TOKEN_RBRACKET)                                                      \
    _X(SH_TOKEN_COMMENT)                                                       \
    _X(SH_TOKEN_RUNBLOCK)                                                      \
    _X(SH_TOKEN_EOF)

typedef enum
{
#define _X(x) x,
    SH_TOKEN_TYPES_ITER(_X)
#undef _X
} shTokenType;

#define SH_KEYWORDS_COUNT 5
extern const char *shlr_keywords[SH_KEYWORDS_COUNT];

typedef struct _shToken
{
    shTokenType type;
    char *value;
    int line;
    int column;
} shToken;

shToken *shToken_Init(shTokenType type, char *value, int line, int column);
void shToken_Free(shToken *token);

#endif
