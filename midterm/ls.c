/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.c
 * 10/10/2016
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

    /* initialize the global variables */
    init(argv[0]);

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

#ifdef DEBUG
    fprintf(stderr, "[DEBUG]\tactive options:\n");
    if (gl_opts.All) fprintf(stderr, ">>\tA\n");
    if (gl_opts.all) fprintf(stderr, ">>\ta\n");
    if (gl_opts.Columns) fprintf(stderr, ">>\tC\n");
    if (gl_opts.ctime_sorted) fprintf(stderr, ">>\tc\n");
    if (gl_opts.directories) fprintf(stderr, ">>\td\n");
    if (gl_opts.F_symbols) fprintf(stderr, ">>\tF\n");
    if (gl_opts.f_unsorted) fprintf(stderr, ">>\tf\n");
    if (gl_opts.human_sizes) fprintf(stderr, ">>\th\n");
    if (gl_opts.inodes) fprintf(stderr, ">>\ti\n");
    if (gl_opts.kilobytes) fprintf(stderr, ">>\tk\n");
    if (gl_opts.long_print) fprintf(stderr, ">>\tl\n");
    if (gl_opts.number_ids) fprintf(stderr, ">>\tn\n");
    if (gl_opts.q_printing) fprintf(stderr, ">>\tq\n");
    if (gl_opts.Recursive) fprintf(stderr, ">>\tR\n");
    if (gl_opts.reverse_sorted) fprintf(stderr, ">>\tr\n");
    if (gl_opts.Size_sorted) fprintf(stderr, ">>\tS\n");
    if (gl_opts.system_blocks) fprintf(stderr, ">>\ts\n");
    if (gl_opts.t_mtime_sorted) fprintf(stderr, ">>\tt\n");
    if (gl_opts.u_atime_sorted) fprintf(stderr, ">>\tu\n");
    if (gl_opts.w_raw) fprintf(stderr, ">>\tw\n");
    if (gl_opts.x_columns) fprintf(stderr, ">>\tx\n");
    if (gl_opts.one_column) fprintf(stderr, ">>\t1\n");
#endif

    /* no target file/directory specified use current dir */
    if (optind == argc)
    {
        if ((targets = (char**)malloc(2 * sizeof(char*))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            return 1;
        }

        if ((targets[0] = strdup(".")) == NULL)
        {
            fprintf(stderr, "%s: unable to strdup: %s\n",
                    gl_progname,
                    strerror(errno));
            free(targets);
            return 1;
        }

        targets[1] = NULL;
        gl_only_cwd = 1;
    }
    else
    {
        i = optind;
        if ((targets = (char**)malloc((argc - i + 1) * sizeof(char*))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            return 1;
        }

        for (j = 0; i < argc; i++, j++)
        {
            if ((targets[j] = strdup(argv[i])) == NULL)
            {
                fprintf(stderr, "%s: unable to strdup: %s\n",
                        gl_progname,
                        strerror(errno));

                while (--j >= 0)
                    free(targets[j]);
                free(targets);
                return 1;
            }
        }
        targets[j] = NULL;
    }

    /* Note: targets is now a list of files/directories terminated by a NULL */

    /* TODO; check R here and make recursive traverse that uses fts_open */
    traverse(targets);

    for (i = 0; targets[i] != NULL; i++)
        free(targets[i]);
    free(targets);
    return 0;
}
