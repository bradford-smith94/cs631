/* Bradford Smith
 * CS 631 Midterm init.c
 * 10/07/2016
 */

#include "ls.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* pre: none, takes in a char* progname
 * post: initializes global variables
 */
void init(char* progname)
{
    /* set program name */
    gl_progname = progname;

    if (getenv("BLOCKSIZE") != NULL)
    {
        if ((gl_blocksize = atoi(getenv("BLOCKSIZE"))) <= 0)
            gl_blocksize = DEFAULT_BLOCKSIZE;
    }
    else
        gl_blocksize = DEFAULT_BLOCKSIZE;

    /* handle TZ environment variable */
    tzset();

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

    if (isatty(/* stdout */1))
    {
        /* terminal defaults */
        gl_opts.Columns = 1;
        gl_opts.q_printing = 1;

        gl_dir_size_summary = 1;
    }
    else
    {
        /* non-terminal defaults */
        gl_opts.w_raw = 1;
        gl_opts.one_column = 1;

        gl_dir_size_summary = 0;
    }
}
