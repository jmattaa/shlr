#include "cli.h"
#include "shlr.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct option long_opts[] = {
    {"dry-run", 0, NULL, 'd'},
    {"version", 0, NULL, 'v'},
    {"help", 0, NULL, 'h'},
};

static const char *descriptions[] = {
    "only print the commands that would be executed",
    "print version information",
    "print this help message",
};

char *shlr_parsecmd(int argc, char **argv)
{
    int opt;
    while ((opt = getopt_long(argc, argv, "dvh", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd':
                shlr_setDryRun();
                break;
            case 'v':
                printf("shlr version %s\n", SHLR_CLI_VERSION);
                exit(0);
                break;
            case 'h':
                shlr_printhelp();
                exit(0);
                break;
        }
    }

    if (optind < argc)
        return argv[optind];
    else
        return NULL;
}

void shlr_printhelp(void)
{
    printf("Usage: shlr [options] [target]\n");
    printf("Options:\n");
    for (int i = 0; long_opts[i].name != NULL; i++)
    {
        if (long_opts[i].val != 1)
            printf("  -%c, --%-28s %s\n", long_opts[i].val, long_opts[i].name,
                   descriptions[i]);
        else
            printf("      --%-28s %s\n", long_opts[i].name, descriptions[i]);
    }
}
