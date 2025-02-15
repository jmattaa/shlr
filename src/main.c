#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *path = "shlrfile";
    size_t src_len;

    const char *src = shlr_utils_readFile(path, &src_len);

    shLexer *lexer = shLexer_Init(src, src_len);
    shParser *parser = shParser_Init(lexer);

    shAstNode *root = shParser_Parse(parser);

    shAstNode_Print(root, 0);

    shParser_Free(parser);
    shAstNode_Free(root);

    return 0;
}
