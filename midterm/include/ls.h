/* Bradford Smith (bsmith8)
 * CS 631 Midterm ls.h
 * 10/11/2016
 */

#ifndef _LS_H_
#define _LS_H_

#define DEFAULT_BLOCKSIZE 512

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

char gl_dir_size_summary;
char gl_exit_code;
char gl_only_cwd;
char* gl_progname;
int gl_blocksize;


int             main(int, char**);

int             cmp(char*, char*);
int             ftscmp(const FTSENT**, const FTSENT**);
struct winsize  get_winsize();
void            init(char*);
void            init_caps();
void            print(char**, int);
void            print_human_size(unsigned long, char);
void            recursive_traverse(char**);
char*           sanitize(char*);
void            sort(char**);
void            traverse(char**);

#endif /* _LS_H_ */
