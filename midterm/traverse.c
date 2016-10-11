/* Bradford Smith (bsmith8)
 * CS 631 Midterm traverse.c
 * 10/11/2016
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

#ifdef DEBUG
        fprintf(stderr, "[DEBUG]\tfound %d dirs\n", num_dirs);
#endif

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
#ifdef DEBUG
                fprintf(stderr, "[DEBUG]\tfound dir: '%s'\n",
                        targets[i]);
#endif
                free(targets[i]);
            }
            dirs[i - dir_idx] = NULL;
            targets[dir_idx] = NULL;

            /* print the non-directory items */
            print(targets);

            for (i = 0; dirs[i] != NULL; i++)
            {
                if (chdir(dirs[i]) < 0)
                {
                    fprintf(stderr, "%s: unable to chdir to '%s': %s\n",
                            gl_progname,
                            dirs[i],
                            strerror(errno));
                    /* TODO: maybe continue if permission error? */
                    exit(1);
                }

                if ((directory = opendir(".")) == NULL)
                {
                    fprintf(stderr, "%s: could not opendir '%s': %s\n",
                            gl_progname,
                            dirs[i],
                            strerror(errno));
                    /* TODO: maybe continue if permission error? */
                    exit(1);
                }

                if (!(gl_only_cwd && strcmp(dirs[i], ".") == 0))
                    printf("\n%s:\n", dirs[i]);

                num_children = 0;
                children = NULL;
                while ((entry = readdir(directory)) != NULL)
                {

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
                                    (num_children + 1) * sizeof(char*))) == NULL)
                    {
                        fprintf(stderr, "%s: unable to malloc: %s\n",
                                gl_progname,
                                strerror(errno));
                        exit(1);
                    }
                    children[num_children - 1] = strdup(entry->d_name);
                }
                if (children != NULL)
                {
                    children[num_children] = NULL;

                    /* TODO: check this from main instead */
                    if (gl_opts.Recursive)
                        traverse(children);
                    else
                    {
                        sort(children);
                        print(children);
                    }

                    /* free the children; say no child slaves! */
                    for (j = 0; children[j] != NULL; j++)
                        free(children[j]);
                    free(children);
                }

                if (chdir("..") < 0)
                {
                    fprintf(stderr, "%s: unable to chdir to '..': %s\n",
                            gl_progname,
                            strerror(errno));
                    exit(1);
                }

                (void)closedir(directory);
            }

            for (i = 0; dirs[i] != NULL; i++)
                free(dirs[i]);
            free(dirs);
        }
        else
            print(targets);
    }
}
