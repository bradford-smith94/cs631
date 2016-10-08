/* Bradford Smith (bsmith8)
 * CS 631 Midterm print.c
 * 10/07/2016
 */

#include "ls.h"

#include <sys/ioctl.h>

#include <stdio.h>
#include <string.h>

void print(char** targets)
{
    int i;
    int x;
    int y;
    int len;
    struct winsize ws;

    /* TODO: i, s, h, k flags need to be checked */
    /* when s is checked also check gl_dir_size_summary */

    if (gl_opts.long_print || gl_opts.number_ids)
    {
        /* TODO: n flag needs to be checked again in here */
    }
    else if (gl_opts.Columns || gl_opts.x_columns)
    {
        ws = get_winsize();
        if (gl_opts.x_columns)
        {
            /* horizontal columns */
            for (i = 0, x = 0; targets[i] != NULL; i++)
            {
                len = strlen(targets[i]);
                if (x + len >= ws.ws_col)
                {
                    x = 0;
                    y++;
                }
                term_move(x, y);
                printf("%s  ", targets[i]);
                x += len + 2;
            }
        }
        else
        {
            /* veritical columns */
        }
    }
    else if (gl_opts.one_column)
    {
        for (i = 0; targets[i] != NULL; i++)
        {
            if (gl_opts.q_printing)
                printf("%s\n", sanitize(targets[i]));
            else
                printf("%s\n", targets[i]);
        }
    }
}
