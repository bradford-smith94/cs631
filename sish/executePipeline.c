/* Bradford Smith (bsmith8)
 * CS 631 sish executePipeline.c
 * 12/15/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

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
                    i = 1;
                    (void)printf("%s", pipeline[0][i++]);
                    while (pipeline[0][i] != NULL)
                    {
                        (void)printf(" %s", pipeline[0][i++]);
                    }
                    (void)printf("\n");
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
        else /* exec */
        {
        }
    }
}
