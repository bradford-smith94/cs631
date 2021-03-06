/* Bradford Smith (bsmith8)
 * CS 631 Midterm sanitize.c
 * 10/11/2016
 */

#include "ls.h"

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
        if ((output = (char*)malloc((len + 1) * sizeof(char))) == NULL)
        {
            fprintf(stderr, "%s: unabel to malloc: %s\n",
                    gl_progname,
                    strerror(errno));
            exit(1);
        }

        for (i = 0; input[i] != '\0'; i++)
        {
            if (input[i] <= 31)
                output[i] = '?';
            else
                output[i] = input[i];
        }
        output[i] = '\0';
    }

    return output;
}
