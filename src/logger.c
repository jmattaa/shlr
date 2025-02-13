#include "logger.h"
#include "lexer/token.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void shlr_logger_log(const char *format, ...)
{
    printf("shlr: ");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void shlr_logger_error(const char *fmt, ...)
{
    fprintf(stderr, "\x1b[31mshlr\x1b[0m: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void shlr_logger_fatal(int err, const char *fmt, ...)
{
    fprintf(stderr, "shlr \x1b[31m[FATAL]\x1b[0m: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(err);
}

#define _X(x) #x,
static const char *shlr_token_type_names[SH_TOKEN_TYPES_COUNT] = {
    SH_TOKEN_TYPES_ITER(_X)};
#undef _X

void shlr_logger_logToken(shToken *token)
{
    printf("token: {%s, '%s', line: %d, col: %d}\n",
           shlr_token_type_names[token->type], token->value, token->line,
           token->column);
}
