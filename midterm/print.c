/* Bradford Smith (bsmith8)
 * CS 631 Midterm print.c
 * 10/06/2016
 */

#include "ls.h"

#include <stdio.h>

void print(char** targets)
{
    int i;

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

    sort(targets);

    for (i = 0; targets[i] != NULL; i++)
    {
        /* TODO: lots of checks and sorting */
        printf("%s\n", targets[i]);
    }
}
