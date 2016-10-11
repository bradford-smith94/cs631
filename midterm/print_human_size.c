/* Bradford Smith (bsmith8)
 * CS 631 Midterm print_human_size.c
 * 10/11/2016
 */

#include "ls.h"

/* pre: takes in an unsigned long 'bytes' and a char 'newline'
 * post: prints out bytes in a human readable format, followed by a newline if
 *      'newline' is not zero
 */
void print_human_size(unsigned long bytes, char newline)
{
    unsigned long size_mod;

    if (bytes >= 1024)
    {
        size_mod = 1;
        while ((bytes / (size_mod * 1024)) >= 1024)
            size_mod++;
    }
    else
        size_mod = 0;

    switch (size_mod)
    {
        case 0: /* bytes */
            if (newline)
                printf("%li\n", bytes);
            else
                printf("%li ", bytes);
            break;
        case 1: /* kilobytes */
            if (newline)
                printf("%liK\n", bytes / (size_mod * 1024));
            else
                printf("%liK ", bytes / (size_mod * 1024));
            break;
        case 2: /* megabytes */
            if (newline)
                printf("%liM\n", bytes / (size_mod * 1024));
            else
                printf("%liM ", bytes / (size_mod * 1024));
            break;
        case 3: /* gigabytes */
            if (newline)
                printf("%liG\n", bytes / (size_mod * 1024));
            else
                printf("%liG ", bytes / (size_mod * 1024));
            break;
        case 4: /* terabytes */
            if (newline)
                printf("%liT\n", bytes / (size_mod * 1024));
            else
                printf("%liT ", bytes / (size_mod * 1024));
            break;
        default: /* ? */
            fprintf(stderr, "%s: unknown size\n", gl_progname);
            break;
    }
}
