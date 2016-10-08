/* Bradford Smith
 * CS 631 Midterm init_caps.c
 * 10/07/2016
 */

#include "ls.h"

#include <stdio.h>
#include <stdlib.h>
#include <term.h> /* XXX curses too? */

/* pre: output is to a terminal
 * post: sets up terminal capabilities
 */
void init_caps()
{
    char bp[2048];
    static char area[2048];
    char* term;

    if ((term = getenv("TERM")) == NULL)
    {
        fprintf(stderr, "%s: no TERM environment variable\n", gl_progname);
        exit(1);
    }

    if (tgetent(bp, term) < 1)
    {
        fprintf(stderr, "%s: could not load terminfo\n", gl_progname);
        exit(1);
    }

    /* the move termcap */
    if (!(gl_term_move = tgetstr("cm", (char**)&area)))
    {
        fprintf(stderr, ":%s: unable to load termcap: cm (move)\n", gl_progname);
        exit(1);
    }
}
