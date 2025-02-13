#ifndef SHLR_LOGGER_H
#define SHLR_LOGGER_H

#include "lexer/token.h"
void shlr_logger_log(const char *fmt, ...);
void shlr_logger_error(const char *fmt, ...);
void shlr_logger_fatal(int err, const char *fmt, ...);

void shlr_logger_logToken(shToken *token);

#endif
