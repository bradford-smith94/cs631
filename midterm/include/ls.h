/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.h
 * 10/06/2016
 */

#ifndef _LS_H_
#define _LS_H_

/* global options structure */
struct s_opts {
    /* use char as a true/false */
    char All;
    char all;
    char Columns;
    char ctime_sorted;
    char directories;
    char F_symbols;
    char f_unsorted;
    char human_sizes;
    char inodes;
    char kilobytes;
    char long_print;
    char number_ids;
    char q_printing;
    char Recursive;
    char reverse_sorted;
    char Size_sorted;
    char system_blocks;
    char t_mtime_sorted;
    char u_atime_sorted;
    char w_raw;
    char x_columns;
    char one_column;
} gl_opts;

/* global program name */
char* gl_progname;

int     main(int, char**);

void    init(char*);
void    print(char**);

#endif /* _LS_H_ */
