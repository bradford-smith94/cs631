/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.c
 * 10/06/2016
 */

#include "ls.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int i;
    int j;
    char opt;
    char** targets;

    /* set program name */
    gl_progname = argv[0];

    /* initialize global option variables to zero */
    gl_opts.All             = 0;
    gl_opts.all             = 0;
    gl_opts.Columns         = 0;
    gl_opts.ctime_sorted    = 0;
    gl_opts.directories     = 0;
    gl_opts.F_symbols       = 0;
    gl_opts.f_unsorted      = 0;
    gl_opts.human_sizes     = 0;
    gl_opts.inodes          = 0;
    gl_opts.kilobytes       = 0;
    gl_opts.long_print      = 0;
    gl_opts.number_ids      = 0;
    gl_opts.q_printing      = 0;
    gl_opts.Recursive       = 0;
    gl_opts.reverse_sorted  = 0;
    gl_opts.Size_sorted     = 0;
    gl_opts.system_blocks   = 0;
    gl_opts.t_mtime_sorted  = 0;
    gl_opts.u_atime_sorted  = 0;
    gl_opts.w_raw           = 0;
    gl_opts.x_columns       = 0;
    gl_opts.one_column      = 0;

    while ((opt = getopt(argc, argv, "AaCcdFfhiklnqRrSstuwx1")) != -1)
    {
        switch (opt)
        {
            case 'A':
                gl_opts.All = 1;
                break;
            case 'a':
                gl_opts.all = 1;
                break;
            case 'C':
                gl_opts.Columns = 1;
                gl_opts.long_print = 0; /* overrides l */
                gl_opts.number_ids = 0; /* overrides n */
                gl_opts.x_columns = 0; /* overrides x */
                gl_opts.one_column = 0; /* overrides 1 */
                break;
            case 'c':
                gl_opts.ctime_sorted = 1;
                gl_opts.u_atime_sorted = 0; /* c overrides u */
                break;
            case 'd':
                gl_opts.directories = 1;
                break;
            case 'F':
                gl_opts.F_symbols = 1;
                break;
            case 'f':
                gl_opts.f_unsorted = 1;
                break;
            case 'h':
                gl_opts.human_sizes = 1;
                gl_opts.kilobytes = 0; /* h overrides k */
                break;
            case 'i':
                gl_opts.inodes = 1;
                break;
            case 'k':
                gl_opts.kilobytes = 1;
                gl_opts.human_sizes = 0; /* k overrides h */
                break;
            case 'l':
                gl_opts.long_print = 1;
                gl_opts.Columns = 0; /* overrides C */
                gl_opts.number_ids = 0; /* overrides n */
                gl_opts.x_columns = 0; /* overrides x */
                gl_opts.one_column = 0; /* overrides 1 */
                break;
            case 'n':
                gl_opts.number_ids = 1;
                gl_opts.Columns = 0; /* overrides C */
                gl_opts.long_print = 0; /* overrides l */
                gl_opts.x_columns = 0; /* overrides x */
                gl_opts.one_column = 0; /* overrides 1 */
                break;
            case 'q':
                gl_opts.q_printing = 1;
                gl_opts.w_raw = 0; /* q overrides w */
                break;
            case 'R':
                gl_opts.Recursive = 1;
                break;
            case 'r':
                gl_opts.reverse_sorted = 1;
                break;
            case 'S':
                gl_opts.Size_sorted = 1;
                break;
            case 's':
                gl_opts.system_blocks = 1;
                break;
            case 't':
                gl_opts.t_mtime_sorted = 1;
                break;
            case 'u':
                gl_opts.u_atime_sorted = 1;
                gl_opts.ctime_sorted = 0; /* u overrides c */
                break;
            case 'w':
                gl_opts.w_raw = 1;
                gl_opts.q_printing = 0; /* w overrides q */
                break;
            case 'x':
                gl_opts.x_columns = 1;
                gl_opts.Columns = 0; /* overrides C */
                gl_opts.long_print = 0; /* overrides l */
                gl_opts.number_ids = 0; /* overrides n */
                gl_opts.one_column = 0; /* overrides 1 */
                break;
            case '1':
                gl_opts.one_column = 1;
                gl_opts.Columns = 0; /* overrides C */
                gl_opts.long_print = 0; /* overrides l */
                gl_opts.number_ids = 0; /* overrides n */
                gl_opts.x_columns = 0; /* overrides x */
                break;
            default:
                fprintf(stderr, "%s: invalid option -- %c\n",
                        gl_progname,
                        opt);
                return 1;
        }
    }

    /* move i to the first non-option argument */
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
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

            /* TODO: check this? */strcpy(targets[j], argv[i]);
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
