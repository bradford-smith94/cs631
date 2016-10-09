/* Bradford Smith
 * CS 631 Midterm sort.c
 * 10/08/2016
 */

#include "ls.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* pre: none, takes in a char** 'list'
 * post: post sorts 'list' based on the defined global options
 */
void sort(char** list)
{
    int i;
    int j;
    char* tmp;

    /* XXX: to use swap give it the address of elements:
        swap(&(list[0]), &(list[1]));
       XXX: might not need swap
    */

    for (i = 1; list[i] != NULL; i++)
    {
        tmp = list[i];

        for (j = i - 1; j >= 0; j--)
        {
            if (cmp(list[j], tmp) > 0)
                list[j + 1] = list[j];
            else
                break;
        }
        list[j + 1] = tmp;
    }
}
