/* Bradford Smith (bsmith8)
 * CS 631 sish restoreIO.c
 * 12/16/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* pre: none
 * post: ensure that stdin and stdout point to their default file descriptors by
 *       restoring them from saved backups
 */
void restoreIO()
{
    (void)close(STDIN_FILENO);
    if (dup2(gl_saved_stdin, STDIN_FILENO) == -1)
    {
        (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    (void)fflush(stdin);

    (void)close(STDOUT_FILENO);
    if (dup2(gl_saved_stdout, STDOUT_FILENO) == -1)
    {
        (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    (void)fflush(stdout);
}
