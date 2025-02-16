#include "lexer/lexer.h"
#include "logger.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *path = "shlrfile.sh";
    size_t src_len;

    const char *src = shlr_utils_readFile(path, &src_len);

    shLexer *lexer = shLexer_Init(src, src_len);

    shToken *token = shLexer_Next(lexer);
    while (token->type != SH_TOKEN_EOF)
    {
        shlr_logger_logToken(token);
        token = shLexer_Next(lexer);
    }

    shToken_Free(token);
    shLexer_Free(lexer);

    return 0;
}
