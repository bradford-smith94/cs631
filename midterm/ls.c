/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.c
 * 09/23/2016
 */

#include "ls.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int i;
    char** targets;

    for (i = 1; i < argc; i++)
    {
        /* if argument starts with a hypen it is an option */
        if (argv[i][0] == '-')
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\tOption: [%s]\n", &argv[i][1]);
#endif
        }
        else
            break;
    }

    /* no target file/directory specified use current dir */
    if (i == argc)
    {
        /* TODO: might need getcwd */

        if ((targets = (char**)malloc(2 * sizeof(char*))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    argv[0],
                    strerror(errno));
            return 1;
        }

        if ((targets[0] = (char*)malloc(strlen(".") * sizeof(char))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    argv[0],
                    strerror(errno));
            free(targets);
            return 1;
        }

        /* TODO: check this */strcpy(targets[0], ".");
        targets[1] = NULL;
    }
    else
    {
        if ((targets = (char**)malloc((argc - i + 1) * sizeof(char*))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    argv[0],
                    strerror(errno));
            return 1;
        }

        for (i; i < argc; i++)
        {
            /* TODO: malloc each target and strcpy it */
        }
        /* TODO: add final NULL to targets */
    }

    return 0;
}
