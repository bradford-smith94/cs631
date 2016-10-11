/* Bradford Smith (bsmith8)
 * CS 631 Midterm recursive_traverse.c
 * 10/11/2016
 */

#include "ls.h"

#include <dirent.h>
#include <fcntl.h>

void recursive_traverse(char** targets)
{
    int i;
    int dir_fd;
    int num_children;
    int num_dirs;
    char** children;
    char** dirs;
    DIR* directory;
    FTS* filesystem;
    FTSENT* entity;

    if (gl_opts.all)
        filesystem = fts_open(targets, FTS_PHYSICAL | FTS_SEEDOT, ftscmp);
    else
        filesystem = fts_open(targets, FTS_PHYSICAL, ftscmp);

    dir_fd = -1;
    num_children = 0;
    num_dirs = 0;
    children = NULL;
    dirs = NULL;
    directory = NULL;
    while ((entity = fts_read(filesystem)) != NULL)
    {
        if ((strcmp(entity->fts_name, ".") == 0 ||
            strcmp(entity->fts_name, "..") == 0) && !gl_opts.all)
        {
            continue;
        }
        else if (entity->fts_name[0] == '.' && (!gl_opts.all && !gl_opts.All))
        {
            continue;
        }
#ifdef DEBUG
        fprintf(stderr, "[DEBUG]\trecursive traversing: '%s'\n",
                entity->fts_path);
#endif

        if (entity->fts_info == FTS_D)
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\trecursive traversing: dir pre\n");
#endif
            /* print any files from previous directory */
            if (children != NULL)
            {
                children[num_children] = NULL;
                num_children = 0;

                if (dir_fd == -1)
                    print(children, AT_FDCWD);
                else
                    print(children, dir_fd);

                for (i = 0; children[i] != NULL; i++)
                    free(children[i]);
                free(children);
                children = NULL;
            }

#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\topenning dir: pre '%s'\n",
                    entity->fts_path);
#endif

            if ((directory = opendir(entity->fts_path)) == NULL)
            {
                fprintf(stderr, "%s: unable to opendir '%s': %s\n",
                        gl_progname,
                        entity->fts_path,
                        strerror(errno));
                gl_exit_code = 1;
                continue;
            }

            if ((dir_fd = dirfd(directory)) < 0)
            {
                fprintf(stderr, "%s: unable to get fd for dir '%s': %s\n",
                        gl_progname,
                        entity->fts_path,
                        strerror(errno));
                exit(1);
            }

            num_dirs++;
            if ((dirs = (char**)realloc(dirs, num_dirs * sizeof(char*))) == NULL)
            {
                fprintf(stderr, "%s: unable to realloc: %s\n",
                        gl_progname,
                        strerror(errno));
                exit(1);
            }
            dirs[num_dirs - 1] = strdup(entity->fts_path);
        }
        else if (entity->fts_info == FTS_DP)
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\trecursive traversing: dir post\n");
#endif
            /* print any files from previous directory */
            if (children != NULL)
            {
                children[num_children] = NULL;
                num_children = 0;

                if (!(gl_only_cwd && strcmp(entity->fts_name, ".") == 0))
                    printf("\n%s:\n", entity->fts_path);

                if (dir_fd == -1)
                    print(children, AT_FDCWD);
                else
                    print(children, dir_fd);

                for (i = 0; children[i] != NULL; i++)
                    free(children[i]);
                free(children);
                children = NULL;
            }

            if (dir_fd != -1)
            {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\tclosing dir: post '%s'\n",
                    entity->fts_path);
#endif
                (void)closedir(directory);
                dir_fd = -1;
                directory = NULL;

                /* pop a directory */
                free(dirs[--num_dirs]);

                /* re-open the last dir */
                if (num_dirs > 0)
                {
                    if ((directory = opendir(dirs[num_dirs - 1])) == NULL)
                    {
                        fprintf(stderr, "%s: unable to opendir '%s': %s\n",
                                gl_progname,
                                dirs[num_dirs - 1],
                                strerror(errno));
                        exit(1);
                    }

                    if ((dir_fd = dirfd(directory)) < 0)
                    {
                        fprintf(stderr, "%s: unable to get fd for dir '%s': %s\n",
                                gl_progname,
                                dirs[num_dirs - 1],
                                strerror(errno));
                        exit(1);
                    }
                }
            }

        }
        else if (entity->fts_info == FTS_DNR)
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\trecursive traversing: dir err\n");
#endif
            fprintf(stderr, "%s: cannot read directory '%s': %s\n",
                    gl_progname,
                    entity->fts_path,
                    strerror(entity->fts_errno));
            gl_exit_code = 1;
            continue;
        }
        else
        {
#ifdef DEBUG
            fprintf(stderr, "[DEBUG]\trecursive traversing: file\n");
#endif
            num_children++;
            if ((children = (char**)realloc(children,
                            (num_children + 1) * sizeof(char*))) == NULL)
            {
                fprintf(stderr, "%s: unable to realloc: %s\n",
                        gl_progname,
                        strerror(errno));
                exit(1);
            }
            children[num_children - 1] = strdup(entity->fts_name);
        }
    }

    while (num_dirs > 0)
        free(dirs[--num_dirs]);
    free(dirs);

    (void)fts_close(filesystem);
}
