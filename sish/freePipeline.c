/* Bradford Smith (bsmith8)
 * CS 631 sish freePipeline.c
 * 12/14/2016
 */

#include "sish.h"

#include <stdlib.h>

void freePipeline(char*** pipeline)
{
    int i;
    int j;

    for (i = 0; pipeline[i] != NULL; i++)
    {
        for (j = 0; pipeline[i][j] != NULL; i++)
        {
            free(pipeline[i][j]);
        }
        free(pipeline[i]);
    }
    free(pipeline);
}
