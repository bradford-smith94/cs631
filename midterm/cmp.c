/* Bradford Smith
 * CS 631 Midterm cmp.c
 * 10/08/2016
 */

#include "ls.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* pre: takes in a char* a, and a char* b which are not NULL
 * post: compares a and b
 * return: an integer less than, equal to, or greater than zero (like strcmp(3))
 *
 * Behaves like strcmp(3) except also takes into account whether or not a or b
 * represent a directory as well as the option flags regarding sorting
 */
int cmp(char* a, char* b)
{
    /* TODO: c, r, S, t, u need to be checked here */
    struct stat sa;
    struct stat sb;

    if (stat(a, &sa) < 0)
    {
        fprintf(stderr, "%s: could not stat '%s': %s\n",
                gl_progname,
                a,
                strerror(errno));
        exit(1);
    }

    if (stat(b, &sb) < 0)
    {
        fprintf(stderr, "%s: could not stat '%s': %s\n",
                gl_progname,
                b,
                strerror(errno));
        exit(1);
    }

    if ((S_ISDIR(sa.st_mode) && S_ISDIR(sb.st_mode)) ||
        (!S_ISDIR(sa.st_mode) && !S_ISDIR(sb.st_mode)))
    {
        return strcmp(a, b);
    }
    else if(S_ISDIR(sa.st_mode))
    {
        return 1;
    }
    else
    {
        return -1;
    }

}
