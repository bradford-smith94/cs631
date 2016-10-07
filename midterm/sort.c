/* Bradford Smith
 * CS 631 Midterm sort.c
 * 10/07/2016
 */

#include "ls.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* pre: none, takes in a char** 'list'
 * post: post sorts 'list' based on the defined global options
 */
void sort(char** list)
{
    struct stat st;
    int i;

    /* XXX: to use swap give it the address of elements:
        swap(&(list[0]), &(list[1]));

       TODO: c, r, S, t, u need to be checked here
    */

    for (i = 0; list[i] != NULL; i++)
    {
        if (stat(list[i], &st) < 0)
        {
            fprintf(stderr, "%s: could not stat '%s': %s\n",
                    gl_progname,
                    list[i],
                    strerror(errno));
            exit(1);
        }

        if (S_ISDIR(st.st_mode))
        {
        }
        else
            continue;
    }
}
