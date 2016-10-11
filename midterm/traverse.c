/* Bradford Smith
 * CS 631 Midterm traverse.c
 * 10/10/2016
 */

#include "ls.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void traverse(char** targets)
{
    int i;
    int j;
    int dir_idx;
    int num_dirs;
    int num_children;
    char** dirs;
    char** children;
    struct stat st;
    DIR* directory;
    struct dirent* entry;

    if (!gl_opts.f_unsorted)
        sort(targets);

    if (gl_opts.directories)
    {
        print(targets);
    }
    else
    {
        /* count and separate out dirs */
        for (i = 0, dir_idx = 0, num_dirs = 0; targets[i] != NULL; i++)
        {
            if (stat(targets[i], &st) < 0)
            {
                fprintf(stderr, "%s: could not stat '%s': %s\n",
                        gl_progname,
                        targets[i],
                        strerror(errno));
                exit(1);
            }

            if (S_ISDIR(st.st_mode))
            {
                if (!dir_idx)
                    dir_idx = i;
                num_dirs++;
            }
        }

        if (num_dirs > 0)
        {
            if ((dirs = (char**)malloc((num_dirs + 1) * sizeof(char*))) == NULL)
            {
                fprintf(stderr, "%s: unable to malloc: %s\n",
                        gl_progname,
                        strerror(errno));
                exit(1);
            }

            for (i = dir_idx; targets[i] != NULL; i++)
            {
                dirs[i - dir_idx] = strdup(targets[i]);
                free(targets[i]);
            }
            dirs[i - dir_idx] = NULL;
            targets[dir_idx] = NULL;

            /* print the non-directory items */
            print(targets);

            for (i = 0; dirs[i] != NULL; i++)
            {
                /* TODO chdir(2) */
                if ((directory = opendir(dirs[i])) == NULL)
                {
                    fprintf(stderr, "%s: could not opendir '%s': %s\n",
                            gl_progname,
                            dirs[i],
                            strerror(errno));
                    /* TODO: maybe continue if permission error? */
                    exit(1);
                }

                num_children = 0;
                children = NULL;
                while ((entry = readdir(directory)) != NULL)
                {
                    if (!gl_only_cwd)
                        printf("\n%s:\n", dirs[i]);

#ifdef DEBUG
                    fprintf(stderr, "[DEBUG]\ttraversing '%s'\n", entry->d_name);
#endif

                    if ((strcmp(entry->d_name, ".") == 0 ||
                        strcmp(entry->d_name, "..") == 0) && !gl_opts.all)
                    {
                        continue;
                    }
                    else if(entry->d_name[0] == '.' && (!gl_opts.all && !gl_opts.All))
                    {
                        continue;
                    }

                    num_children++;
                    if ((children = (char**)realloc(children,
                                    num_children * sizeof(char*) + 1)) == NULL)
                    {
                        fprintf(stderr, "%s: unable to malloc: %s\n",
                                gl_progname,
                                strerror(errno));
                        exit(1);
                    }
                    children[num_children - 1] = strdup(entry->d_name);
                }
                if (children != NULL)
                    children[num_children - 1] = NULL;

                /* TODO: check R maybe use fts_open from main instead? */
                sort(children);
                print(children);

                if (children != NULL)
                {
                    /* free the children; say no child slaves! */
                    for (j = 0; children[j] != NULL; j++)
                        free(children[j]);
                    free(children);
                }
            }

            for (i = 0; dirs[i] != NULL; i++)
                free(dirs[i]);
            free(dirs);
        }
        else
            print(targets);
    }
}
