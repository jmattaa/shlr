#include "cli.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct option long_opts[] = {
    {"dry-run", 0, NULL, 'd'},
    {"list-targets", 0, NULL, 'l'},
    {"list-dependencies", 0, NULL, 'L'},
    {"version", 0, NULL, 'v'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0},
};

static const char *descriptions[] = {
    "only print the commands that would be executed",
    "list all the available targets",
    "like --list-targets, but also print the dependencies for each target",
    "print version information",
    "print this help message",
};

shlrCli *shlr_parsecmd(int argc, char **argv)
{
    shlrCli *cli = malloc(sizeof(shlrCli));

    cli->dryrun = 0;
    cli->listtargs = 0;
    cli->listdeps = 0;
    cli->targ = NULL;

    int opt;
    while ((opt = getopt_long(argc, argv, "dlLvh", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd':
                cli->dryrun = 1;
                break;
            case 'l':
                cli->listtargs = 1;
                break;
            case 'L':
                cli->listtargs = 1;
                cli->listdeps = 1;
                break;
            case 'v':
                printf("shlr version %s\n", SHLR_CLI_VERSION);
                exit(0);
                break;
            case 'h':
                shlr_printhelp();
                exit(0);
                break;
            default:
                shlr_printhelp();
                exit(1);
        }
    }

    if (optind < argc)
        cli->targ = argv[optind];

    return cli;
}

void shlr_freecli(shlrCli *cli)
{
    free(cli);
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
