#ifndef SHLR_SHLR_H
#define SHLR_SHLR_H

#include "parser/AST.h"

char *shlr_createTargScript(shAstNode *root, char *targ);
void shlr_runScript(char *script);

#endif
