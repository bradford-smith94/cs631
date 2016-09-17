/* Bradford Smith
 * CS 631 HW 2 tcp.c
 * 09/17/2016
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_SEP '/'

int main(int argc, char** argv)
{
    char* src;
    char* dest;
    char* fulldest;
    int srcfd;
    int destfd;
    struct stat* srcstat;
    struct stat* deststat;

    if (argc < 3) //expecting a [src] and [dest]
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

    if ((srcstat = (struct stat*)malloc(sizeof(struct stat))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                argv[0],
                strerror(errno));
        return 1;
    }

    if ((deststat = (struct stat*)malloc(sizeof(struct stat))) == NULL)
    {
        fprintf(stderr, "%s: unable to malloc: %s\n",
                argv[0],
                strerror(errno));
        return 1;
    }

    if (stat(src, srcstat) < 0)
    {
        fprintf(stderr, "%s: cannot stat '%s': %s\n",
                argv[0],
                src,
                strerror(errno));
        return 1;
    }

    if ((srcstat->st_mode & S_IFMT) == S_IFDIR)
    {
        fprintf(stderr, "%s: '%s' is a directory\n",
                argv[0],
                src);
        return 1;
    }

    //if dest exists and is a directory
    if ((stat(dest, deststat) == 0) && ((deststat->st_mode & S_IFMT) == S_IFDIR))
    {
        fulldest = (char*)malloc(strlen(dest) + strlen(src) + 1 * sizeof(char));
        if ((sprintf(fulldest, "%s%c%s", dest, PATH_SEP, src)) < 0)
        {
            fprintf(stderr, "%s: unable to create destination filename: %s\n",
                    argv[0],
                    strerror(errno));
            return 1;
        }

        if ((destfd = open(fulldest, O_WRONLY|O_CREAT, 00644)) < 0)
        {
            fprintf(stderr, "%s: unable to open '%s': %s\n",
                    argv[0],
                    fulldest,
                    strerror(errno));
            return 1;
        }
    }
    else
    {
        if ((destfd = open(dest, O_WRONLY|O_CREAT, 00644)) < 0)
        {
            fprintf(stderr, "%s: unable to open '%s': %s\n",
                    argv[0],
                    dest,
                    strerror(errno));
            return 1;
        }
    }

    if ((srcfd = open(src, O_RDONLY)) < 0)
    {
        fprintf(stderr, "%s: unable to open '%s': %s\n",
                argv[0],
                src,
                strerror(errno));
        return 1;
    }

    (void)close(srcfd);
    (void)close(destfd);

    return 0;
}
