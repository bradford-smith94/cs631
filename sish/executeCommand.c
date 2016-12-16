/* Bradford Smith (bsmith8)
 * CS 631 sish executeCommand.c
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

/* pre: takes in a char** 'command' which represents the command to execute as
 *      a char* array
 * post: execute the command represented by 'command'
 */
void executeCommand(char** command)
{
    int status;
    pid_t pid;

    redirectIO(command);

    if (!strcmp(command[0], CD))
    {
        if (command[1] != NULL)
        {
            if (chdir(command[1]) == -1)
            {
                (void)fprintf(stderr, "%s: %s\n", CD, strerror(errno));
                gl_exit_code = EXIT_FAILURE;
            }
            else
                gl_exit_code = EXIT_SUCCESS;
        }
        else
        {
            if (chdir(gl_home_path) == -1)
            {
                (void)fprintf(stderr, "%s: %s\n", CD, strerror(errno));
                gl_exit_code = EXIT_FAILURE;
            }
            else
                gl_exit_code = EXIT_SUCCESS;
        }
    }
    else if (!strcmp(command[0], ECHO))
    {
        if (command[1] != NULL)
        {
            if (!strcmp(command[1], CURRENT_PID_STR))
            {
                (void)printf("%d\n", gl_current_pid);
            }
            else if (!strcmp(command[1], EXIT_STATUS_STR))
            {
                (void)printf("%d\n", gl_exit_code);
            }
            else
            {
                (void)printf("%s\n", command[1]);
            }
        }
        else
        {
            (void)printf("\n");
        }

        /* echo success */
        gl_exit_code = EXIT_SUCCESS;
    }
    else if (!strcmp(command[0], EXIT))
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
            if (execvp(command[0], &command[0]) == -1)
            {
                (void)fprintf(stderr, "%s: %s: command not found\n",
                              getprogname(),
                              command[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    restoreIO();
}
