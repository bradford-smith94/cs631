/* Bradford Smith (bsmith8)
 * CS 631 HW 2 Extra tcpm.c
 * 09/19/2016
 */

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_SEP '/'

int main(int argc, char** argv)
{
    char* bsrc;         /* base source (just filename) */
    char* dest;         /* destination arg */
    char* fdest;        /* full destination path */
    char* src;          /* source arg */
    int dfd;            /* destination file descriptor */
    int sfd;            /* source file descriptor */
    struct stat* dstat; /* destination stat output */
    struct stat* sstat; /* source stat output */
    void* dmap;         /* destination memory map */
    void* smap;         /* source memory map */

    if (argc < 3) /* expecting a [src] and [dest] */
    {
        fprintf(stderr, "%s: missing arguments\n", argv[0]);
        fprintf(stderr, "usage: %s [src] [dest]\n", argv[0]);
        return 1;
    }
    else if (argc > 3)
    {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        fprintf(stderr, "usage: %s [src] [dest]\n", argv[0]);
        return 1;
    }

    src = argv[1];
    dest = argv[2];

#ifdef DEBUG
    fprintf(stderr, "[DEBUG]\tsource: [%s] dest: [%s]\n",
            src,
            dest);
#endif

    if ((sstat = (struct stat*)malloc(sizeof(struct stat))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                argv[0],
                strerror(errno));
        return 1;
    }

    if ((dstat = (struct stat*)malloc(sizeof(struct stat))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                argv[0],
                strerror(errno));
        (void)free(sstat);
        return 1;
    }

    if (stat(src, sstat) < 0)
    {
        fprintf(stderr, "%s: cannot stat '%s': %s\n",
                argv[0],
                src,
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        return 1;
    }

    if ((sstat->st_mode & S_IFMT) == S_IFDIR)
    {
        fprintf(stderr, "%s: '%s' is a directory\n",
                argv[0],
                src);
        (void)free(sstat);
        (void)free(dstat);
        return 1;
    }

    /* if dest exists and is a directory */
    if ((stat(dest, dstat) == 0) && ((dstat->st_mode & S_IFMT) == S_IFDIR))
    {
        if ((bsrc = basename(strdup(src))) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc source filename: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            return 1;
        }

        fdest = (char*)malloc((strlen(dest) + strlen(bsrc) + 1) * sizeof(char));
        if (fdest == NULL)
        {
            fprintf(stderr, "%s: unable to malloc destination filename: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            return 1;
        }

        if ((sprintf(fdest, "%s%c%s", dest, PATH_SEP, bsrc)) < 0)
        {
            fprintf(stderr, "%s: unable to create destination filename: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            (void)free(fdest);
            return 1;
        }
#ifdef DEBUG
        fprintf(stderr, "[DEBUG]\tdest is a directory, full dest: [%s]\n",
                fdest);
#endif
    }
    else
    {
        bsrc = NULL;

        if ((fdest = strdup(dest)) == NULL)
        {
            fprintf(stderr, "%s: unable to malloc destination filename: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            return 1;
        }
    }

#ifdef DEBUG
    fprintf(stderr, "[DEBUG]\tsource inode: [%d] dest inode: [%d]\n",
            sstat->st_ino,
            dstat->st_ino);
#endif

    if ((sstat->st_dev) == (dstat->st_dev) && (sstat->st_ino) == (dstat->st_ino))
    {
        fprintf(stderr, "%s: '%s' and '%s' are the same file\n",
                argv[0],
                src,
                dest);
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);
        return 1;
    }

    /* NOTE: mmap with PROT_WRITE and MAP_SHARED requires O_RDWR */
    /* O_TRUNC will set file size to zero if it already exists */
    if ((dfd = open(fdest, O_RDWR|O_CREAT|O_TRUNC, 00644)) < 0)
    {
        fprintf(stderr, "%s: unable to open '%s': %s\n",
                argv[0],
                dest,
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);
        return 1;
    }

    /* make sure destination file has space for source file */
    if (ftruncate(dfd, sstat->st_size) < 0)
    {
        fprintf(stderr, "%s: unable to ftruncate destination file: %s\n",
                argv[0],
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);
        return 1;
    }

    if ((sfd = open(src, O_RDONLY)) < 0)
    {
        fprintf(stderr, "%s: unable to open '%s': %s\n",
                argv[0],
                src,
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);

        (void)close(dfd);
        return 1;
    }

    /* mmap will fail if size is zero */
    if (sstat->st_size != 0)
    {
        if ((dmap = mmap(NULL, sstat->st_size, PROT_WRITE, MAP_SHARED, dfd, 0)) == MAP_FAILED)
        {
            fprintf(stderr, "%s: unable to map destination file to memory: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            (void)free(fdest);

            (void)close(dfd);
            (void)close(sfd);
            return 1;
        }

        if ((smap = mmap(NULL, sstat->st_size, PROT_READ, MAP_PRIVATE, sfd, 0)) == MAP_FAILED)
        {
            fprintf(stderr, "%s: unable to map source file to memory: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            (void)free(fdest);

            (void)close(dfd);
            (void)close(sfd);

            (void)munmap(dmap, sstat->st_size);
            return 1;
        }

#ifdef DEBUG
        fprintf(stderr, "[DEBUG]\tBefore memcpy\n");
#endif

        (void)memcpy(dmap, smap, sstat->st_size);

#ifdef DEBUG
        fprintf(stderr, "[DEBUG]\tAfter memcpy\n");
#endif
    }

    (void)free(sstat);
    (void)free(dstat);
    (void)free(fdest);

    (void)close(dfd);
    (void)close(sfd);

    (void)munmap(dmap, sstat->st_size);
    (void)munmap(smap, sstat->st_size);
    return 0;
}
