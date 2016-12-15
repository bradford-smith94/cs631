/* Bradford Smith
 * CS 631 sish handleSigInt.c
 * 12/14/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

/* pre: none
 * post: handle a SIGINT by skipping to the next line
 */
void handleSigInt()
{
    /* reestablish signal handler in case it gets set to default */
    if (signal(SIGINT, handleSigInt) == SIG_ERR)
    {
        (void)fprintf(stderr, "%s: signal failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    (void)printf("\n");
    (void)printf(PROMPT_STR);
    (void)fflush(stdout);
}
