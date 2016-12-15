/* Bradford Smith (bsmith8)
 * CS 631 sish init.c
 * 12/15/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <sys/types.h>

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* pre: none
 * post: initialize global variables and environment
 */
void init()
{
    char path[PATH_MAX];
    char realpath[PATH_MAX];

    gl_current_pid = getpid();
    gl_exit_code = EXIT_SUCCESS;
    gl_trace = 0;

    if ((gl_saved_stdin = dup(STDIN_FILENO)) == -1)
    {
        (void)fprintf(stderr, "%s: dup failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((gl_saved_stdout = dup(STDOUT_FILENO)) == -1)
    {
        (void)fprintf(stderr, "%s: dup failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    setprogname(NAME);

    if (signal(SIGINT, handleSigInt) == SIG_ERR)
    {
        (void)fprintf(stderr, "%s: signal failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* find executable path using /proc/<pid>/exe and set $SHELL */
    (void)sprintf(path, "/proc/%d/exe", gl_current_pid);
    if (readlink(path, realpath, PATH_MAX) == -1)
    {
        (void)fprintf(stderr, "%s: readlink failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (setenv(ENV_SHELL, realpath, 1) == -1)
    {
        (void)fprintf(stderr, "%s: setenv failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((gl_home_path = getenv(ENV_HOME)) == NULL)
    {
        (void)fprintf(stderr, "%s: getenv failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }
}
