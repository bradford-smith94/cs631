/* Bradford Smith
 * CS 631 sish init.c
 * 12/14/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* pre: none
 * post: initialize global variables
 */
void init()
{
    gl_exit_code = EXIT_SUCCESS;
    gl_trace = 0;

    setprogname(NAME);

    if (signal(SIGINT, handleSigInt) == SIG_ERR)
    {
        (void)fprintf(stderr, "%s: signal failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }
}
