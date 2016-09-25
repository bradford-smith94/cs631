/* Bradford Smith
 * CS 631 Midterm defineOpt.c
 * 09/25/2016
 */

#include "ls.h"

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

/* pre: none, takes in a char 'opt'
 * post: set 'opt' in the global options structure
 */
void defineOpt(char opt)
{
#ifdef DEBUG
    fprintf(stderr, "[DEBUG]\tattempting to define option '%c'\n", opt);
#endif

    if (index(ACCEPTED_OPTS, opt) == NULL)
    {
        fprintf(stderr, "%s: invalid option -- '%c'\n", gl_progname, opt);
        exit(1);
    }

    if (opt >= 'A' && opt <= 'Z')
    {
        /* upper */
        gl_opts.upper[opt - 'A'] = 1;
    }
    else if (opt >= 'a' && opt <= 'z')
    {
        /* lower */
        gl_opts.lower[opt - 'a'] = 1;
    }
    else if (opt >= '0' && opt <= '9')
    {
        /* digit */
        gl_opts.digit[opt - '0'] = 1;
    }
}
