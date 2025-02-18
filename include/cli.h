#ifndef SHLR_CLI_H
#define SHLR_CLI_H

#define SHLR_CLI_VERSION "0.2.1"

typedef struct _shlrCli
{
    int dryrun;
    int listtargs;
    int listdeps;
    char *targ;
} shlrCli;

shlrCli *shlr_parsecmd(int argc, char **argv);
void shlr_freecli(shlrCli *cli);

void shlr_printhelp(void);

#endif
