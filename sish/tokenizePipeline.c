/* Bradford Smith (bsmith8)
 * CS 631 sish tokenizePipeline.c
 * 12/19/2016
 */

#include "sish.h"

#include <bsd/stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pre: takes in a char* 'cmd'
 * post: tokenizes 'cmd' and executes it accordingly
 * return: a malloc'd char*** that represents the entire command pipeline as an
 *         array of char* arrays -- that is a char* array (char**) for each
 *         pipeline segment (separated by pipe characters)
 */
char*** tokenizePipeline(char* cmd)
{
    char* token;
    char*** pipeline;
    int pipeSegment;
    int word;
    int i;
    int j;

    pipeSegment = 0;
    word = 0;

    pipeline = NULL;
    if ((pipeline =
         realloc(pipeline, (pipeSegment + 2) * sizeof(char**))) == NULL)
    {
        (void)fprintf(stderr, "%s: realloc failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    pipeline[0] = NULL;
    if ((pipeline[0] =
         realloc(pipeline[0], (word + 2) * sizeof(char*))) == NULL)
    {
        (void)fprintf(stderr, "%s: realloc failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    token = strtok(cmd, " \t");
    if ((pipeline[pipeSegment][word++] = strdup(token)) == NULL)
    {
        (void)fprintf(stderr, "%s: strdup failure: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((token = strtok(NULL, " \t")) != NULL)
    {
        if (!strcmp(token, "|"))
        {
            /* end current pipe segment vector and increment */
            pipeline[pipeSegment++][word] = NULL;

            if ((pipeline =
                 realloc(pipeline, (pipeSegment + 2) * sizeof(char**))) == NULL)
            {
                (void)fprintf(stderr, "%s: realloc failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }

            pipeline[pipeSegment] = NULL;
            word = 0;
        }
        else
        {
            if ((pipeline[pipeSegment][word++] = strdup(token)) == NULL)
            {
                (void)fprintf(stderr, "%s: strdup failure: %s\n",
                              getprogname(),
                              strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        if ((pipeline[pipeSegment] =
             realloc(pipeline[pipeSegment], (word + 2) * sizeof(char*))) ==
             NULL)
        {
            (void)fprintf(stderr, "%s: realloc failure: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    pipeline[pipeSegment][word] = NULL;
    pipeline[pipeSegment + 1] = NULL;

    if (gl_trace)
    {
        for (i = 0; pipeline[i] != NULL; i++)
        {
            (void)fprintf(stderr, "+");
            for (j = 0; pipeline[i][j] != NULL; j++)
                (void)fprintf(stderr, " %s", pipeline[i][j]);
            (void)fprintf(stderr, "\n");
        }
    }

    return pipeline;
}
