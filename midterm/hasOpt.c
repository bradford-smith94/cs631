/* Bradford Smith (bsmith8)
 * CS 631 Midterm hasOpt.c
 * 09/26/2016
 */

#include "ls.h"

/* pre: option strings have been parsed, takes in a char 'opt'
 * post: checks if 'opt' has been defined via arguments to main
 * return: 1 if 'opt' is defined, else 0
 */
int hasOpt(char opt)
{
    if (opt >= 'A' && opt <= 'Z')
    {
        /* upper */
        return gl_opts.upper[opt - 'A'] == 1;
    }
    else if (opt >= 'a' && opt <= 'z')
    {
        /* lower */
        return gl_opts.lower[opt - 'a'] == 1;
    }
    else if (opt >= '0' && opt <= '9')
    {
        /* digit */
        return gl_opts.digit[opt - '0'] == 1;
    }
    else
    {
        return 0;
    }
}
