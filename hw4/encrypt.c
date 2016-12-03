/* Bradford Smith (bsmith8)
 * CS 631 HW 4 encrypt.c
 * 12/03/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>

#include <openssl/evp.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void encrypt()
{
    char buf[BUFSIZ];
    unsigned char out[BUFSIZ];
    int n;
    int out_n;
    int total;
    t_privates privs;
    EVP_CIPHER_CTX ctx;

    privs = genkey(NULL);

    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), privs.key, privs.iv);

    bzero(buf, BUFSIZ);
    bzero(out, BUFSIZ);

    while ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
    {
        bzero(out, BUFSIZ);

        if (!EVP_EncryptUpdate(&ctx, out, &out_n, (unsigned char*)buf, n))
        {
            (void)fprintf(stderr, "%s: error in EVP_EncryptUpdate\n",
                          getprogname());
            exit(EXIT_FAILURE);
        }
        if (!EVP_EncryptFinal(&ctx, out + out_n, &total))
        {
            (void)fprintf(stderr, "%s: error in EVP_EncryptFinal\n",
                          getprogname());
            exit(EXIT_FAILURE);
        }

        out_n += total;

        n = 0;
        while ((n = write(STDOUT_FILENO, out + n, out_n - n)) > 0); /* NOTE ; */
        if (n == -1)
        {
            (void)fprintf(stderr, "%s: write error: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }

        bzero(buf, BUFSIZ);
    }
    if (n == -1)
    {
        (void)fprintf(stderr, "%s: read error: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX_cleanup(&ctx);
}
