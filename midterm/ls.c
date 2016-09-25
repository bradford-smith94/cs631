/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.c
 * 09/25/2016
 */

#include "ls.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int i;              /* loop counter */
    int j;              /* loop counter */
    char** targets;     /* the files/directories to print */

    /* make sure all options are initially zero */
    bzero(gl_opts.upper, 26);
    bzero(gl_opts.lower, 26);
    bzero(gl_opts.digit, 10);

    /* set program name */
    gl_progname = argv[0];

    for (i = 1; i < argc; i++)
    {
        /* if argument starts with a hypen it is an option */
        if (argv[i][0] == '-')
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\tOption: [%s]\n", &argv[i][1]);
#endif
            /* loop through option string with j and defineOpt() each char */
            for (j = 0; j < strlen(&argv[i][1]); j++)
            {
                defineOpt(argv[i][1 + j]);
            }
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
                    gl_progname,
                    strerror(errno));
            return 1;
        }

        if ((targets[0] = (char*)malloc(strlen(".") * sizeof(char))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            free(targets);
            return 1;
        }

        /* TODO: check this? */strcpy(targets[0], ".");
        targets[1] = NULL;
    }
    else
    {
        if ((targets = (char**)malloc((argc - i + 1) * sizeof(char*))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            return 1;
        }

        /* leave `i` where it is */
        for (j = 0; i < argc; i++, j++) /* Note: `j` is incremented here */
        {
            if ((targets[j] = (char*)malloc(strlen(argv[i]) * sizeof(char))) == NULL)
            {
                fprintf(stderr, "%s: unable to maloc: %s\n",
                        gl_progname,
                        strerror(errno));
                free(targets);
                return 1;
            }
        }
        targets[j] = NULL;
    }

    /* Note: targets is now a list of files/directories terminated by a NULL */
    print(targets);

    for (i = 0; targets[i] != NULL; i++)
        free(targets[i]);
    free(targets);
    return 0;
}
