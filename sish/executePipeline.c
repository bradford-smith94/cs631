/* Bradford Smith (bsmith8)
 * CS 631 sish executePipeline.c
 * 12/15/2016
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
    int status;
    pid_t pid;

    /* count pipeline segments */
    for (i = 0; pipeline[i] != NULL; i++);

    if (i > 1) /* pipeline */
    {
    }
    else /* single command */
    {
        if (!strcmp(pipeline[0][0], CD))
        {
            if (pipeline[0][1] != NULL)
            {
                if (chdir(pipeline[0][1]) == -1)
                {
                    (void)fprintf(stderr, "%s: %s\n",
                                  CD,
                                  strerror(errno));
                    gl_exit_code = EXIT_FAILURE;
                }
                else
                    gl_exit_code = EXIT_SUCCESS;
            }
            else
            {
                if (chdir(gl_home_path) == -1)
                {
                    (void)fprintf(stderr, "%s: %s\n",
                                  CD,
                                  strerror(errno));
                    gl_exit_code = EXIT_FAILURE;
                }
                else
                    gl_exit_code = EXIT_SUCCESS;
            }
        }
        else if (!strcmp(pipeline[0][0], ECHO))
        {
            if (pipeline[0][1] != NULL)
            {
                if (!strcmp(pipeline[0][1], CURRENT_PID_STR))
                {
                    (void)printf("%d\n", gl_current_pid);
                }
                else if (!strcmp(pipeline[0][1], EXIT_STATUS_STR))
                {
                    (void)printf("%d\n", gl_exit_code);
                }
                else
                {
                    (void)printf("%s\n", pipeline[0][1]);
                }
            }
            else
            {
                (void)printf("\n");
            }

            /* echo success */
            gl_exit_code = EXIT_SUCCESS;
        }
        else if (!strcmp(pipeline[0][0], EXIT))
        {
            exit(gl_exit_code);
        }
        else /* fork and exec */
        {
            if ((pid = fork()) < 0)
            {
                (void)fprintf(stderr, "%s: fork failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                status = EXIT_SUCCESS;
                waitpid(pid, &status, 0);
                gl_exit_code = WEXITSTATUS(status);
            }
            else
            {
                if (execvp(pipeline[0][0], &pipeline[0][0]) == -1)
                {
                    (void)fprintf(stderr, "%s: %s: %s\n",
                                  getprogname(),
                                  pipeline[0][0],
                                  strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
