#include <stdio.h>
#include <unistd.h>
/* going above and beyond */
#include <string.h>
#include <errno.h>
#include <stdlib.h>
/* ~~~~ */

int
main(int argc, char **argv) {

    /* going above and beyond */
    char* user;
    if ((user = getlogin()) == NULL) {
        fprintf(stderr,"unable to getlogin(): %s\n", strerror(errno));
        exit(-1);
    }
    /* ~~~~ */

    printf("Welcome to CS631 Advanced Programming in the UNIX Environment, %s!\n", user);
}
