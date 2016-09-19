/* Bradford Smith (bsmith8)
 * CS 631 HW 2 tcp.c
 * 09/19/2016
 */

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_SEP '/'

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

int main(int argc, char** argv)
{
    char* bsrc;         /* base source (just filename) */
    char* buf;          /* copy buffer */
    char* dest;         /* destination arg */
    char* fdest;        /* full destination path */
    char* src;          /* source arg */
    int dfd;            /* destination file descriptor */
    int n;              /* num bytes read/write */
    int sfd;            /* source file descriptor */
    struct stat* dstat; /* destination stat output */
    struct stat* sstat; /* source stat output */

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

    if ((dfd = open(fdest, O_WRONLY|O_CREAT|O_TRUNC, 00644)) < 0)
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

    if ((buf = (char*)malloc((BUF_SIZE + 1) * sizeof(char))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                argv[0],
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);

        (void)close(dfd);
        (void)close(sfd);
        return 1;
    }

    while ((n = read(sfd, buf, BUF_SIZE)) > 0)
    {
        if ((n = write(dfd, buf, n)) < 0)
        {
            fprintf(stderr, "%s: unable to write: %s\n",
                    argv[0],
                    strerror(errno));
            (void)free(sstat);
            (void)free(dstat);
            (void)free(fdest);
            (void)free(buf);

            (void)close(dfd);
            (void)close(sfd);
            return 1;
        }
    }
    if (n < 0)
    {
        fprintf(stderr, "%s: unable to read: %s\n",
                argv[0],
                strerror(errno));
        (void)free(sstat);
        (void)free(dstat);
        (void)free(fdest);
        (void)free(buf);

        (void)close(dfd);
        (void)close(sfd);
        return 1;
    }

    (void)free(sstat);
    (void)free(dstat);
    (void)free(fdest);
    (void)free(buf);

    (void)close(dfd);
    (void)close(sfd);
    return 0;
}
