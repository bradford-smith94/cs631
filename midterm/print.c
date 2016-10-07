/* Bradford Smith (bsmith8)
 * CS 631 Midterm print.c
 * 10/07/2016
 */

#include "ls.h"

#include <stdio.h>

void print(char** targets)
{
    int i;

    /* TODO: i, s, h, k flags need to be checked */

    for (i = 0; targets[i] != NULL; i++)
    {
        if (gl_opts.long_print || gl_opts.number_ids)
        {
            /* TODO: n flag needs to be checked again in here */
        }
        else if (gl_opts.Columns || gl_opts.x_columns)
        {
            /* TODO (optional) */
            /* TODO: x (optional) flag needs to be checked again in here */
        }
        else if (gl_opts.one_column)
        {
            if (gl_opts.q_printing)
                printf("%s\n", sanitize(targets[i]));
            else
                printf("%s\n", targets[i]);
        }
    }
}
