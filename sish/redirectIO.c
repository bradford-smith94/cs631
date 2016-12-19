/* Bradford Smith (bsmith8)
 * CS 631 sish redirectIO.c
 * 12/19/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* pre: takes in a char** 'command' which represents a single command string and
 *      all it's arguments
 * post: checks 'command' for I/O redirection control characters and performs
 *       the necesary redirections
 */
void redirectIO(char** command)
{
    int i;
    int fd;
    int in_len;
    int out_len;
    int append_len;

    in_len = strlen(IO_IN);
    out_len = strlen(IO_OUT);
    append_len = strlen(IO_OUT_APPEND);

    /* TODO: check for illegal combinations */
    for (i = 1; command[i] != NULL; i++)
    {
        if (!strncmp(command[i], IO_IN, in_len))
        {
            if ((fd = open(&command[i][in_len], O_RDONLY)) == -1)
            {
                (void)fprintf(stderr, "%s: %s: '%s'\n",
                              getprogname(),
                              strerror(errno),
                              &command[i][in_len]);
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDIN_FILENO) == -1)
            {
                (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }

            (void)close(fd);
            removeIndex(command, i--); /* decrement i so as not to skip next */
        }
        else if (!strncmp(command[i], IO_OUT_APPEND, append_len))
        {
            if ((fd =
                 open(&command[i][append_len],
                      O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
            {
                (void)fprintf(stderr, "%s: %s: '%s'\n",
                              getprogname(),
                              strerror(errno),
                              &command[i][append_len]);
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }

            (void)close(fd);
            removeIndex(command, i--); /* decrement i so as not to skip next */
        }
        else if (!strncmp(command[i], IO_OUT, out_len))
        {
            if ((fd =
                 open(&command[i][out_len],
                      O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
            {
                (void)fprintf(stderr, "%s: %s: '%s'\n",
                              getprogname(),
                              strerror(errno),
                              &command[i][out_len]);
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }

            (void)close(fd);
            removeIndex(command, i--); /* decrement i so as not to skip next */
        }
    }
}
