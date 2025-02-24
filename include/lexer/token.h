#ifndef SHLR_LEXER_TOKEN_H
#define SHLR_LEXER_TOKEN_H

#define SH_TOKEN_TYPES_COUNT 6
#define SH_TOKEN_TYPES_ITER(_X)                                                \
    _X(SH_TOKEN_KEYWORD)                                                       \
    _X(SH_TOKEN_IDENTIFIER)                                                    \
    _X(SH_TOKEN_COMMENT)                                                       \
    _X(SH_TOKEN_RUNBLOCK)                                                      \
    _X(SH_TOKEN_EOF)                                                           \
    _X(SH_TOKEN_UNEXPECTED)

typedef enum
{
#define _X(x) x,
    SH_TOKEN_TYPES_ITER(_X)
#undef _X
} shTokenType;

#define SH_KEYWORDS_COUNT 3
#define SH_KEYWORDS_ITER(_X)                                                   \
    _X(targ)                                                                   \
    _X(depends)                                                                \
    _X(endtarg)

typedef enum
{
#define _X(x) SH_KEYWORD_##x,
    SH_KEYWORDS_ITER(_X)
#undef _X
} shKeywordType;

extern const char *shlr_keywords[SH_KEYWORDS_COUNT];
extern const char *shlr_token_type_names[SH_TOKEN_TYPES_COUNT];

typedef struct _shToken
{
    shTokenType type;
    char *value;
    int line;
    int column;

    // only if it's a keyword
    shKeywordType keyword;
} shToken;

shToken *shToken_Init(shTokenType type, char *value, int line, int column);
void shToken_Free(shToken *token);

#endif
