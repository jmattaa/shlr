#ifndef SHLR_CLI_H
#define SHLR_CLI_H

#define SHLR_CLI_VERSION "0.1.0"

// parse the command and return the target script
char *shlr_parsecmd(int argc, char **argv);
void shlr_printhelp(void);

#endif
