/* Bradford Smith (bsmith8)
 * CS 631 Midterm sort.c
 * 10/11/2016
 */

#include "ls.h"

/* pre: none, takes in a char** 'list'
 * post: post sorts 'list' based on the defined global options
 */
void sort(char** list)
{
    int i;
    int j;
    char* tmp;

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
