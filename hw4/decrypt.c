/* Bradford Smith (bsmith8)
 * CS 631 HW 4 decrypt.c
 * 12/04/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>

#include <openssl/err.h>
#include <openssl/evp.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void decrypt()
{
    char buf[BUFSIZ];
    unsigned char out[BUFSIZ];
    char tmp[SALT_SIZE];
    int len;
    int n;
    int out_n;
    int total;
    t_privates* privs;
    EVP_CIPHER_CTX ctx;

    bzero(buf, BUFSIZ);

    /* read in the prefix */
    len = strlen(ENC_PREFIX) + SALT_SIZE;
    n = 0;
    total = 0;
    while ((len -= n))
    {
        if ((n = read(STDIN_FILENO, buf + total, len)) > 0)
        {
            total += n;
        }
        else if (n == -1)
        {
            (void)fprintf(stderr, "%s: read error: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            (void)fprintf(stderr, "%s: invalid input\n",
                          getprogname());
            exit(EXIT_FAILURE);
        }
    }

    if (strncmp(buf, ENC_PREFIX, strlen(ENC_PREFIX)))
    {
        (void)fprintf(stderr, "%s: invalid input\n",
                      getprogname());
        exit(EXIT_FAILURE);
    }

    (void)memcpy(tmp, (buf + strlen(ENC_PREFIX)), SALT_SIZE);
    privs = genkey((unsigned char*)tmp);

    EVP_CIPHER_CTX_init(&ctx);
    EVP_DecryptInit(&ctx, EVP_aes_256_cbc(), privs->key, privs->iv);
    len = EVP_CIPHER_block_size(EVP_aes_256_cbc());

    bzero(buf, BUFSIZ);

    /* decrypt and write stdin */
    while ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
    {
        bzero(out, BUFSIZ);

        out_n = 0;
        if (!EVP_DecryptUpdate(&ctx, out, &out_n, (unsigned char*)buf, n))
        {
            (void)fprintf(stderr, "%s: error in EVP_DecryptUpdate: %s\n",
                          getprogname(),
                          ERR_error_string(ERR_get_error(), NULL));
            exit(EXIT_FAILURE);
        }

        total = 0;
        if (!EVP_DecryptFinal(&ctx, out + out_n, &total))
        {
            (void)fprintf(stderr, "%s: error in EVP_DecryptFinal: %s\n",
                          getprogname(),
                          ERR_error_string(ERR_get_error(), NULL));
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

    free(privs);

    EVP_CIPHER_CTX_cleanup(&ctx);
}
