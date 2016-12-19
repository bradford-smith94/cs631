/* Bradford Smith (bsmith8)
 * CS 631 sish main.c
 * 12/19/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int opt;
    int i;
    int n;
    size_t len;
    char* command;
    char*** pipeline;

    init();

    command = NULL;
    while ((opt = getopt(argc, argv, "c:x")) != -1)
    {
        switch (opt)
        {
            case 'c':
                if ((command = strdup(optarg)) == NULL)
                {
                    (void)fprintf(stderr, "%s: strdup failure: %s\n",
                                  getprogname(),
                                  strerror(errno));
                    return EXIT_FAILURE;
                }
                break;
            case 'x':
                gl_trace = 1;
                break;
            case '?':
                return EXIT_FAILURE;
        }
    }
    /* TODO add check that no other parameters are given? */

    if (command != NULL)
    {
        pipeline = tokenizePipeline(command);
        executePipeline(pipeline);
        freePipeline(pipeline);
    }
    else
    {
        while (1)
        {
            (void)printf(PROMPT_STR);
            len = 0;
            errno = 0;
            if ((n = getline(&command, &len, stdin)) < 0)
            {
                if (errno)
                {
                    (void)fprintf(stderr, "%s: getline failure: %s\n",
                                  getprogname(),
                                  strerror(errno));
                    return EXIT_FAILURE;
                }
                else
                {
                    (void)fprintf(stderr, "%s: getline: unexpected EOF\n",
                                  getprogname());
                    return EXIT_FAILURE;
                }
            }

            command[n - 1] = '\0'; /* remove newline */
            if (strlen(command))
            {
                pipeline = tokenizePipeline(command);
                executePipeline(pipeline);
                freePipeline(pipeline);
            }

            free(command);
            command = NULL;

            /* if there are background pids check if they exited */
            if (gl_bg_pids[0] != 0)
            {
                for (i = 0; gl_bg_pids[i] != 0; i++)
                {
                    n = EXIT_SUCCESS;
                    waitpid(gl_bg_pids[i], &n, WNOHANG);
                    if (WIFEXITED(n))
                        gl_exit_code = WEXITSTATUS(n);
                }
            }
        }
    }

    return gl_exit_code;
}
