/* Bradford Smith (bsmith8)
 * CS 631 HW 4 genkey.c
 * 12/04/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_privates* genkey(unsigned char* givenSalt)
{
    char* aed_pass;
    int i;
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char salt[SALT_SIZE];
    t_privates* ret;

    if ((ret = (t_privates*)malloc(sizeof(t_privates))) == NULL)
    {
        (void)fprintf(stderr, "%s: could not malloc: %s\n",
                      getprogname(),
                      strerror(errno));
        exit(EXIT_FAILURE);

    }

    bzero(ret->key, EVP_MAX_KEY_LENGTH);
    bzero(ret->iv, EVP_MAX_IV_LENGTH);
    bzero(ret->salt, SALT_SIZE);

    if ((aed_pass = getenv(AED_PASS)) == NULL)
    {
        (void)fprintf(stderr, "%s: could not get environment variable '%s'\n",
                      getprogname(),
                      AED_PASS);
        exit(EXIT_FAILURE);
    }

    if (givenSalt == NULL)
    {
        if (!RAND_bytes(salt, SALT_SIZE))
        {
            (void)fprintf(stderr, "%s: RAND_bytes failed: %s\n",
                          getprogname(),
                          ERR_error_string(ERR_get_error(), NULL));
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < SALT_SIZE; i++)
            ret->salt[i] = salt[i];
    }
    else
    {
        for (i = 0; i < SALT_SIZE; i++)
            ret->salt[i] = givenSalt[i];
    }

    if (!EVP_BytesToKey(EVP_aes_256_cbc(),
                        EVP_sha1(),
                        ret->salt,
                        (unsigned char*)aed_pass,
                        strlen(aed_pass),
                        1,
                        key,
                        iv))
    {
        (void)fprintf(stderr, "%s: EVP_BytesToKey failed\n",
                      getprogname());
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < EVP_MAX_KEY_LENGTH; i++)
        ret->key[i] = key[i];
    for (i = 0; i < EVP_MAX_IV_LENGTH; i++)
        ret->iv[i] = iv[i];

    return ret;
}
