/* Bradford Smith (bsmith8)
 * CS 631 HW 4 encrypt.c
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

void encrypt()
{
    char buf[BUFSIZ];
    unsigned char out[BUFSIZ];
    char* tmp;
    int i;
    int len;
    int n;
    int out_n;
    int total;
    t_privates* privs;
    EVP_CIPHER_CTX ctx;

    privs = genkey(NULL);

    /* write out the prefix */
    if ((tmp = strdup(ENC_PREFIX)) == NULL)
    {
        (void)fprintf(stderr, "%s: strdup error: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < strlen(ENC_PREFIX); i++)
    {
        if (write(STDOUT_FILENO, &tmp[i], 1) == -1)
        {
            (void)fprintf(stderr, "%s: write error: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < SALT_SIZE; i++)
    {
        if (write(STDOUT_FILENO, &privs->salt[i], 1) == -1)
        {
            (void)fprintf(stderr, "%s: write error: %s\n",
                          getprogname(),
                          strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    free(tmp);

    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), privs->key, privs->iv);
    len = EVP_CIPHER_block_size(EVP_aes_256_cbc());

    bzero(buf, BUFSIZ);

    /* encrypt and write stdin */
    while ((n = read(STDIN_FILENO, buf, len)) > 0)
    {
        if (n < len)
        {
            /* make sure we got the whole block or are at the end */
            total = 0;
            do
            {
                total += n;
            } while ((n = read(STDIN_FILENO, buf + total, len - total)) > 0);
            n = total;
        }

        bzero(out, BUFSIZ);

        if (!EVP_EncryptUpdate(&ctx, out, &out_n, (unsigned char*)buf, n))
        {
            (void)fprintf(stderr, "%s: error in EVP_EncryptUpdate: %s\n",
                          getprogname(),
                          ERR_error_string(ERR_get_error(), NULL));
            exit(EXIT_FAILURE);
        }

        if (!EVP_EncryptFinal(&ctx, out + out_n, &total))
        {
            (void)fprintf(stderr, "%s: error in EVP_EncryptFinal: %s\n",
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
