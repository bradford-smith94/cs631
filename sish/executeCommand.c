/* Bradford Smith (bsmith8)
 * CS 631 sish executeCommand.c
 * 12/17/2016
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
 *      a char* array and an int 'nofork', 'nofork' should be nonzero if calling
 *      this function from a child process
 * post: execute the command represented by 'command', if 'nofork' is nonzero
 *       this will not fork a new process for the command
 */
void executeCommand(char** command, int nofork)
{
    int status;
    pid_t pid;

    if (!strcmp(command[0], CD))
    {
        if (command[1] != NULL)
        {
            if (chdir(command[1]) == -1)
            {
                (void)fprintf(stderr, "%s: %s\n", CD, strerror(errno));
                if (nofork)
                    exit(EXIT_NO_EXEC);
                gl_exit_code = EXIT_NO_EXEC;
            }
            else
            {
                if (nofork)
                    exit(EXIT_SUCCESS);
                gl_exit_code = EXIT_SUCCESS;
            }
        }
        else
        {
            if (chdir(gl_home_path) == -1)
            {
                (void)fprintf(stderr, "%s: %s\n", CD, strerror(errno));
                if (nofork)
                    exit(EXIT_NO_EXEC);
                gl_exit_code = EXIT_NO_EXEC;
            }
            else
            {
                if (nofork)
                    exit(EXIT_SUCCESS);
                gl_exit_code = EXIT_SUCCESS;
            }
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
        if (nofork)
            exit(EXIT_SUCCESS);
        gl_exit_code = EXIT_SUCCESS;
    }
    else if (!strcmp(command[0], EXIT))
    {
        /* exit probably shouldn't be used in pipeline child process */
        exit(gl_exit_code);
    }
    else /* fork and exec */
    {
        if (nofork)
        {
            if (execvp(command[0], &command[0]) == -1)
            {
                (void)fprintf(stderr, "%s: %s: command not found\n",
                              getprogname(),
                              command[0]);
                exit(EXIT_NO_EXEC);
            }
        }

        if ((pid = fork()) < 0)
        {
            (void)fprintf(stderr, "%s: fork failure: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) /* parent */
        {
            status = EXIT_SUCCESS;
            waitpid(pid, &status, 0);
            gl_exit_code = WEXITSTATUS(status);
        }
        else /* child */
        {
            if (execvp(command[0], &command[0]) == -1)
            {
                (void)fprintf(stderr, "%s: %s: command not found\n",
                              getprogname(),
                              command[0]);
                exit(EXIT_NO_EXEC);
            }
        }
    }
}
