/* Bradford Smith
 * CS 631 Midterm traverse.c
 * 10/10/2016
 */

#include "ls.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void traverse(char** targets)
{
    int i;
    int dir_idx;
    int num_dirs;
    int num_children;
    char** dirs;
    char** children;
    struct stat st;
    FTS* filesystem;
    FTSENT* parent;
    FTSENT* child;

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

            /* TODO: check this for failure */
            if (gl_opts.all)
                filesystem = fts_open(dirs, FTS_PHYSICAL|FTS_NOSTAT|FTS_SEEDOT, NULL);
            else
                filesystem = fts_open(dirs, FTS_PHYSICAL|FTS_NOSTAT, NULL);

            while ((parent = fts_read(filesystem)) != NULL)
            {
                if (!gl_only_cwd)
                    printf("\n%s:\n", parent->fts_name);

                child = fts_children(filesystem, FTS_NAMEONLY);

                if (child != NULL)
                {
                    /* TODO: check A */
                    num_children = 1;
                    while (child->fts_link != NULL)
                    {
                        num_children++;
                        child = child->fts_link;
                    }

                    /* start from beginning */
                    child = fts_children(filesystem, FTS_NAMEONLY);

                    if ((children = (char**)malloc((num_children + 1) * sizeof(char*)))
                            == NULL)
                    {
                        fprintf(stderr, "%s: unable to malloc: %s\n",
                                gl_progname,
                                strerror(errno));
                        exit(1);
                    }

                    children[0] = strdup(child->fts_name);
                    i = 1;
                    while (child->fts_link != NULL)
                    {
                        child = child->fts_link;
                        children[i++] = strdup(child->fts_name);
                    }
                    children[i] = NULL;

                    if (gl_opts.Recursive)
                    {
                        /* traverse children */
                        traverse(children);
                    }
                    else
                    {
                        /* sort and print children */
                        if (!gl_opts.f_unsorted)
                            sort(children);

                        print(children);
                    }

                    /* free the children; say no child slaves! */
                    for (i = 0; children[i] != NULL; i++)
                        free(children[i]);
                    free(children);
                }
            }

            (void)fts_close(filesystem);
            for (i = 0; dirs[i] != NULL; i++)
                free(dirs[i]);
            free(dirs);
        }
        else
            print(targets);
    }
}
