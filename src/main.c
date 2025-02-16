#include "cli.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "shlr.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *targ = shlr_parsecmd(argc, argv);

    char *path = "shlrfile.sh";
    size_t src_len;

    const char *src = shlr_utils_readFile(path, &src_len);

    shLexer *lexer = shLexer_Init(src, src_len);
    shParser *parser = shParser_Init(lexer);

    shAstNode *root = shParser_Parse(parser);

    char *script = shlr_createTargScript(root, targ);
    if (shlr_dryRun)
        printf("%s\n", script);
    else
        shlr_runScript(script);

    free(script);
    shLexer_Free(lexer);
    shParser_Free(parser);
    shAstNode_Free(root);

    return 0;
}
