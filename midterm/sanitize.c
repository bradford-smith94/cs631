/* Bradford Smith
 * CS 631 Midterm sanitize.c
 * 10/07/2016
 */

#include "ls.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pre: none, takes in a char* 'input'
 * post: sanitizes 'input' by replacing all the non-printing characters it
 *      contains
 * return: a char* which is 'input' but with non-printing characters replaced by
 *      '?' characters
 */
char* sanitize(char* input)
{
    int i;
    int len;
    char* output;

    output = NULL;
    if (input != NULL)
    {
        len = strlen(input);
        if ((output = (char*)malloc(len * sizeof(char))) == NULL)
        {
            fprintf(stderr, "%s: unabel to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            exit(1);
        }

        for (i = 0; input[i] != '\n'; i++)
        {
            if (input[i] <= 31)
                output[i] = '?';
            else
                output[i] = input[i];
        }
        output[i] = '\n';
    }

    return output;
}
