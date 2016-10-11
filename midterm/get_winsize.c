/* Bradford Smith (bsmith8)
 * CS 631 Midterm get_winsize.c
 * 10/07/2016
 */

#include "ls.h"

#include <sys/ioctl.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct winsize get_winsize()
{
    struct winsize w;
    if (ioctl(0, TIOCGWINSZ, &w) < 0)
    {
        fprintf(stderr, "%s: could not get window size: %s\n",
                gl_progname,
                strerror(errno));
        exit(1);
    }

    return w;
}
