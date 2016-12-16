/* Bradford Smith (bsmith8)
 * CS 631 sish executePipeline.c
 * 12/16/2016
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

/* pre: takes in a char*** 'pipeline' which represents the command(s) to execute
 *      as an array of char* arrays, each char* array representing an entire
 *      command string
 * post: execute the command(s) represented by 'pipeline'
 */
void executePipeline(char*** pipeline)
{
    int i;
    int prevIn;
    int pipeSegments;
    int status;
    int pipeFd[2];
    pid_t pid;
    pid_t* pids;

    /* count pipeline segments, note semicolon */
    for (pipeSegments = 0; pipeline[pipeSegments] != NULL; pipeSegments++);

    if (pipeSegments > 1) /* pipeline */
    {
        if ((pids = (pid_t*)malloc((pipeSegments + 1) * sizeof(pid_t))) == NULL)
        {
            (void)fprintf(stderr, "%s: malloc failure: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }

        prevIn = STDIN_FILENO;

        for (i = 0; i < pipeSegments; i++)
        {
            if (pipe(pipeFd) == -1)
            {
                (void)fprintf(stderr, "%s: pipe failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }

            if ((pid = fork())  < 0)
            {
                (void)fprintf(stderr, "%s: fork failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }
            else if (pid > 0) /* parent */
            {
                pids[i] = pid;
                (void)close(pipeFd[1]);
                prevIn = pipeFd[0];
            }
            else /* child */
            {
                if (prevIn != STDIN_FILENO)
                {
                    if (dup2(prevIn, STDIN_FILENO) == -1)
                    {
                        (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                                      getprogname(),
                                      strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                    (void)close(prevIn);
                }

                if (i != pipeSegments - 1)
                {
                    if (pipeFd[1] != STDOUT_FILENO)
                    {
                        if (dup2(pipeFd[1], STDOUT_FILENO) == -1)
                        {
                            (void)fprintf(stderr, "%s: dup2 failure: %s\n",
                                          getprogname(),
                                          strerror(errno));
                            exit(EXIT_FAILURE);
                        }
                        (void)close(pipeFd[1]);
                    }
                }

                executeCommand(pipeline[i], 1);
            }
        }
        pids[i] = (pid_t)0;

        for (i = 0; pids[i] != 0; i++)
        {
            status = EXIT_SUCCESS;
            waitpid(pid, &status, 0);
            gl_exit_code = WEXITSTATUS(status);
        }

        restoreIO();
    }
    else /* single command */
    {
        redirectIO(pipeline[0]);
        executeCommand(pipeline[0], 0);
        restoreIO();
    }
}
