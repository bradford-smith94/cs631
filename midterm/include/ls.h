/* Bradford Smith (bsmith8)
 * CS 631 midterm ls.h
 * 09/25/2016
 */

#ifndef _LS_H_
#define _LS_H_

#define ACCEPTED_OPTS "AaCcdFfhiklnqRrSstuwx1"

/* global options structure */
struct s_opts {
    char upper[26];
    char lower[26];
    char digit[10];
} gl_opts;

/* global program name */
char* gl_progname;

int     main(int, char**);

void    defineOpt(char);
int     hasOpt(char);
void    print(char**);

#endif /* _LS_H_ */
