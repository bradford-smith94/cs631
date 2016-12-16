/* Bradford Smith (bsmith8)
 * CS 631 sish executePipeline.c
 * 12/16/2016
 */

#include "sish.h"

#include <stdlib.h>

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
        executeCommand(pipeline[0]);
    }
}
