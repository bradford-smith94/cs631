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
    unsigned long total_size;
    struct stat st;
    struct winsize ws;
    struct group* tmp_gr;
    struct passwd* tmp_pw;
    file_info* info;

    /* handle if given nothing */
    if (targets == NULL || targets[0] == NULL)
        return;

    /* TODO: F flags need to be checked */

    /* Note the ';' loop intentionally empty */
    for (i = 0, num_targets = 0; targets[i] != NULL; i++, num_targets++);

    if ((info = (file_info*)malloc(num_targets * sizeof(file_info))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                gl_progname,
                strerror(errno));
        exit(1);
    }

#ifdef DEBUG
    if (containing_dir == AT_FDCWD)
        fprintf(stderr, "[DEBUG]\tprint called with containing_dir == AT_FDCWD\n");
    else
        fprintf(stderr, "[DEBUG]\tprint called with containing_dir\n");
#endif


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
        if (gl_opts.system_blocks && gl_dir_size_summary)
        {
            for (i = 0, total_size = 0; targets[i] != NULL; i++)
                total_size += info[i].blocksize;

            if (gl_opts.human_sizes)
            {
                printf("total: ");
                print_human_size(total_size * gl_blocksize, 1);
            }
            else if (gl_opts.kilobytes)
                printf("total: %li\n", (total_size * gl_blocksize) / 1024);
            else
                printf("total: %li\n", total_size);
        }

        for (i = 0; targets[i] != NULL; i++)
        {
            strftime(date, 13, "%b %d %H:%M",
                    localtime(&info[i].date_modified.tv_sec));

            if (gl_opts.q_printing)
                name = sanitize(targets[i]);
            else
                name = strdup(targets[i]);

            if (gl_opts.inodes)
                printf("%li ", info[i].inode);
            if (gl_opts.system_blocks)
            {
                if (gl_opts.human_sizes)
                {
                    print_human_size(info[i].blocksize * gl_blocksize, 0);
                }
                else if (gl_opts.kilobytes)
                    printf("%li ", (info[i].blocksize * gl_blocksize) / 1024);
                else
                    printf("%li ", info[i].blocksize);
            }

            if (gl_opts.number_ids)
            {
                printf("%s ", info[i].mode);
                printf("%d ", info[i].num_links);
                printf("%li ", info[i].uid);
                printf("%li ", info[i].gid);
                if (gl_opts.human_sizes)
                {
                    print_human_size(info[i].filesize, 0);
                }
                else
                    printf("%li ", info[i].filesize);
                printf("%s ", date);
                printf("%s\n", name);
            }
            else
            {
                printf("%s ", info[i].mode);
                printf("%d ", info[i].num_links);
                printf("%s ", info[i].owner);
                printf("%s ", info[i].group);
                if (gl_opts.human_sizes)
                {
                    print_human_size(info[i].filesize, 0);
                }
                else
                    printf("%li ", info[i].filesize);
                printf("%s ", date);
                printf("%s\n", name);
            }

            free(name);
        }
    }
    else if (gl_opts.Columns || gl_opts.x_columns)
    {
        if (gl_opts.system_blocks && gl_dir_size_summary)
        {
            for (i = 0, total_size = 0; targets[i] != NULL; i++)
                total_size += info[i].blocksize;

            if (gl_opts.human_sizes)
            {
                printf("total: ");
                print_human_size(total_size * gl_blocksize, 1);
            }
            else if (gl_opts.kilobytes)
                printf("total: %li\n", (total_size * gl_blocksize) / 1024);
            else
                printf("total: %li\n", total_size);
        }

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
                if (gl_opts.inodes)
                    printf("%li ", info[i].inode);
                if (gl_opts.system_blocks)
                {
                    if (gl_opts.human_sizes)
                    {
                        print_human_size(info[i].blocksize * gl_blocksize, 0);
                    }
                    else if (gl_opts.kilobytes)
                        printf("%li ", (info[i].blocksize * gl_blocksize) / 1024);
                    else
                        printf("%li ", info[i].blocksize);
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

                    if (gl_opts.inodes)
                        printf("%li ", info[i].inode);
                    if (gl_opts.system_blocks)
                    {
                        if (gl_opts.human_sizes)
                        {
                            print_human_size(info[i].blocksize * gl_blocksize, 0);
                        }
                        else if (gl_opts.kilobytes)
                            printf("%li ", (info[i].blocksize * gl_blocksize) / 1024);
                        else
                            printf("%li ", info[i].blocksize);
                    }

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

                    if (gl_opts.inodes)
                        printf("%li ", info[i].inode);
                    if (gl_opts.system_blocks)
                    {
                        if (gl_opts.human_sizes)
                        {
                            print_human_size(info[i].blocksize * gl_blocksize, 0);
                        }
                        else if (gl_opts.kilobytes)
                            printf("%li ", (info[i].blocksize * gl_blocksize) / 1024);
                        else
                            printf("%li ", info[i].blocksize);
                    }

                    printf("%s\n", name);
                    free(name);
                }
            }
        }
    }
    else if (gl_opts.one_column)
    {
        if (gl_opts.system_blocks && gl_dir_size_summary)
        {
            for (i = 0, total_size = 0; targets[i] != NULL; i++)
                total_size += info[i].blocksize;

            if (gl_opts.human_sizes)
            {
                printf("total: ");
                print_human_size(total_size * gl_blocksize, 1);
            }
            else if (gl_opts.kilobytes)
                printf("total: %li\n", (total_size * gl_blocksize) / 1024);
            else
                printf("total: %li\n", total_size);
        }

        for (i = 0; targets[i] != NULL; i++)
        {
            if (gl_opts.q_printing)
                name = sanitize(targets[i]);
            else
                name = strdup(targets[i]);

            if (gl_opts.inodes)
                printf("%li ", info[i].inode);
            if (gl_opts.system_blocks)
            {
                if (gl_opts.human_sizes)
                {
                    print_human_size(info[i].blocksize * gl_blocksize, 0);
                }
                else if (gl_opts.kilobytes)
                    printf("%li ", (info[i].blocksize * gl_blocksize) / 1024);
                else
                    printf("%li ", info[i].blocksize);
            }

            printf("%s\n", name);
            free(name);
        }
    }

    free(info);
}
