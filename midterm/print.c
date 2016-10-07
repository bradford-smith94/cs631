/* Bradford Smith (bsmith8)
 * CS 631 Midterm print.c
 * 10/07/2016
 */

#include "ls.h"

#include <stdio.h>

void print(char** targets)
{
    int i;

    for (i = 0; targets[i] != NULL; i++)
    {
        /* TODO: lots of checks and sorting */
        printf("%s\n", targets[i]);
    }
}
