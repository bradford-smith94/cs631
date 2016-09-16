/* Bradford Smith
 * CS 631 HW 2 tcp.c
 * 09/16/2016
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
    char* src;
    char* dest;
    int srcfd;
    int destfd;

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

    //TODO: check src is a file

    if ((srcfd = open(src, O_RDONLY)) < 0)
    {
        fprintf(stderr, "unable to open '%s': %s\n",
                src,
                strerror(errno));
        return 1;
    }

    //TODO: check dest is a file or directory

    if ((destfd = open(dest, O_RDWR|O_CREAT)) < 0)
    {
        fprintf(stderr, "unable to open '%s': %s\n",
                dest,
                strerror(errno));
        return 1;
    }

    return 0;
}
