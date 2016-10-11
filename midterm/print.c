/* Bradford Smith (bsmith8)
 * CS 631 Midterm print.c
 * 10/11/2016
 */

#include "ls.h"

#include <sys/ioctl.h>

#include <bsd/string.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

typedef struct s_file {
    unsigned long inode;
    unsigned long blocksize;
    char mode[12]; /* 12 is length returned by strmode(3) */
    int num_links;
    char* owner;
    unsigned long uid;
    char* group;
    unsigned long gid;
    unsigned long filesize;
    struct timespec date_modified;
} file_info;

void print(char** targets, int containing_dir)
{
    int i;
    int x;
    int len;
    int num_targets;
    char* name;
    char date[13]; /* 3 for month, 2 for day, 6 for time, 2 spaces and null */
    struct stat st;
    struct winsize ws;
    struct group* tmp_gr;
    struct passwd* tmp_pw;
    file_info* info;

    /* handle if given nothing */
    if (targets == NULL || targets[0] == NULL)
        return;

    /* TODO: i, s, h, k, F flags need to be checked */
    /* when s is checked also check gl_dir_size_summary */

    /* Note the ';' loop intentionally empty */
    for (i = 0, num_targets = 0; targets[i] != NULL; i++, num_targets++);

    if ((info = (file_info*)malloc(num_targets * sizeof(file_info))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                gl_progname,
                strerror(errno));
        exit(1);
    }

    for (i = 0; targets[i] != NULL; i++)
    {
        if (fstatat(containing_dir, targets[i], &st, 0) < 0)
        {
            fprintf(stderr, "%s: could not stat '%s': %s\n",
                    gl_progname,
                    targets[i],
                    strerror(errno));
            free(info);
            exit(-1);
        }

        info[i].inode = st.st_ino;
        info[i].blocksize = (st.st_blocks * DEFAULT_BLOCKSIZE) / gl_blocksize;
        strmode(st.st_mode, info[i].mode);
        info[i].num_links = st.st_nlink;
        info[i].filesize = st.st_size;
        if ((tmp_pw = getpwuid(st.st_uid)) == NULL)
            info[i].owner = NULL;
        else
            info[i].owner = tmp_pw->pw_name;
        info[i].uid = st.st_uid;
        if ((tmp_gr = getgrgid(st.st_gid)) == NULL)
            info[i].group = NULL;
        else
            info[i].group = tmp_gr->gr_name;
        info[i].gid = st.st_gid;
        info[i].date_modified = st.st_mtim;
    }

    if (gl_opts.long_print || gl_opts.number_ids)
    {
        for (i = 0; targets[i] != NULL; i++)
        {
            strftime(date, 13, "%b %d %H:%M",
                    localtime(&info[i].date_modified.tv_sec));

            if (gl_opts.q_printing)
                name = sanitize(targets[i]);
            else
                name = strdup(targets[i]);

            if (gl_opts.number_ids)
            {
                printf("%s %d %li %li %li %s %s\n",
                        info[i].mode,
                        info[i].num_links,
                        info[i].uid,
                        info[i].gid,
                        info[i].filesize,
                        date,
                        name);
            }
            else
            {
                printf("%s %d %s %s %li %s %s\n",
                        info[i].mode,
                        info[i].num_links,
                        info[i].owner,
                        info[i].group,
                        info[i].filesize,
                        date,
                        name);
            }

            free(name);
        }
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

                if (gl_opts.q_printing)
                    name = sanitize(targets[i]);
                else
                    name = strdup(targets[i]);

                if (x >= ws.ws_col)
                {
                    x = 0;
                }
                else if (x + len >= ws.ws_col)
                {
                    x = 0;
                    printf("\n");
                }
                printf("%s  ", name);
                free(name);
                x += len + 2;
            }

            /* print final newline */
            printf("\n");
        }
        else
        {
            /* veritical columns */

            /* Note ';' loop intentionally empty */
            for (i = 0, x = 0; targets[i] != NULL; x += strlen(targets[i]) + 2, i++);

            /* if all items fit in one row */
            if (x < ws.ws_col)
            {
                for (i = 0; targets[i] != NULL; i++)
                {
                    if (gl_opts.q_printing)
                        name = sanitize(targets[i]);
                    else
                        name = strdup(targets[i]);

                    printf("%s  ", name);
                    free(name);
                }

                /* print final newline */
                printf("\n");
            }
            else
            {
                /* TODO: multiple columns and multiple lines */

                /* if only one column fits */
                for (i = 0; targets[i] != NULL; i++)
                {
                    if (gl_opts.q_printing)
                        name = sanitize(targets[i]);
                    else
                        name = strdup(targets[i]);

                    printf("%s\n", name);
                    free(name);
                }
            }
        }
    }
    else if (gl_opts.one_column)
    {
        for (i = 0; targets[i] != NULL; i++)
        {
            if (gl_opts.q_printing)
                name = sanitize(targets[i]);
            else
                name = strdup(targets[i]);

            printf("%s\n", name);
            free(name);
        }
    }

    free(info);
}
