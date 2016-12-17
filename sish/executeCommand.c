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
 *      a char* array, an int 'nofork', 'nofork' should be nonzero if calling
 *      this function from a child process, and an int 'bg'
 * post: execute the command represented by 'command', if 'nofork' is nonzero
 *       this will not fork a new process for the command, and if 'bg' is
 *       nonzero 'command' will be executed in the background
 */
void executeCommand(char** command, int nofork, int bg)
{
    int status;
    pid_t pid;

    if (!strcmp(command[0], CD)) /* ignores bg, happens immediately */
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
    else if (!strcmp(command[0], ECHO)) /* ignores bg, happens immediately */
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
        if (bg) /* backgrounded exit just exits a subprocess, skip it */
            gl_exit_code = EXIT_SUCCESS;
        else /* if used in a pipeline exit only exits the subprocess */
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
            if (bg) /* add pid to gl_bg_pids */
            {
                gl_num_bg_pids++;
                if ((gl_bg_pids =
                     (pid_t*)
                     realloc(gl_bg_pids, (gl_num_bg_pids + 1) * sizeof(pid_t)))
                     == NULL)
                {
                    (void)fprintf(stderr, "%s: realloc failure: %s\n",
                                  getprogname(),
                                  strerror(errno));
                    exit(EXIT_FAILURE);
                }

                gl_bg_pids[gl_num_bg_pids - 1] = pid;
                gl_bg_pids[gl_num_bg_pids] = (pid_t)0;

                gl_exit_code = status;
            }
            else /* wait for pid */
            {
                waitpid(pid, &status, 0);
                gl_exit_code = WEXITSTATUS(status);
            }
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
