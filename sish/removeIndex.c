/* Bradford Smith (bsmith8)
 * CS 631 sish removeIndex.c
 * 12/16/2016
 */

#include "sish.h"

#include <stdlib.h>

/* pre: takes in a char** 'list' that is allocated and an int 'index'
 * post: frees the element in 'list' at 'index' and shifts the rest of 'list'
 *       into it's place
 */
void removeIndex(char** list, int index)
{
    int i;

    free(list[index]);
    for (i = index + 1; list[i] != NULL; i++)
    {
        list[i - 1] = list[i];
    }
    list[i - 1] = list[i];
}
