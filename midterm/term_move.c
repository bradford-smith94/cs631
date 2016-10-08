/* Bradford Smith
 * CS 631 Midterm term_move.c
 * 10/07/2016
 */

#include "ls.h"

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <term.h>

void term_move(int x, int y)
{
    if (putp(tgoto(gl_term_move, x, y)) == ERR)
    {
        fprintf(stderr, "%s: could not move terminal position\n", gl_progname);
        exit(1);
    }
}
