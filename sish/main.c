/* Bradford Smith (bsmith8)
 * CS 631 sish main.c
 * 12/14/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int opt;
    size_t len;
    char* command;

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
        }
    }
    /* TODO add check that no other parameters are given? */

    if (command != NULL)
    {
    }
    else
    {
        while (1)
        {
            (void)printf(PROMPT_STR);
            len = 0;
            errno = 0;
            if (getline(&command, &len, stdin) < 0)
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

            /* handle command */

            free(command);
            command = NULL;
        }
    }

    return gl_exit_code;
}
