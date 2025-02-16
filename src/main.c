#include "lexer/lexer.h"
#include "parser/parser.h"
#include "shlr.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *targ = NULL;
    if (argc > 1)
        targ = argv[1];

    char *path = "shlrfile.sh";
    size_t src_len;

    const char *src = shlr_utils_readFile(path, &src_len);

    shLexer *lexer = shLexer_Init(src, src_len);
    shParser *parser = shParser_Init(lexer);

    shAstNode *root = shParser_Parse(parser);

    char *script = shlr_createTargScript(root, targ);
    printf("%s\n", script);

    shLexer_Free(lexer);
    shParser_Free(parser);
    shAstNode_Free(root);

    return 0;
}
