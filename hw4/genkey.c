/* Bradford Smith (bsmith8)
 * CS 631 HW 4 genkey.c
 * 12/03/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_privates genkey(unsigned char* givenSalt)
{
    char* aed_pass;
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char salt[8];
    t_privates ret;

    if ((aed_pass = getenv(AED_PASS)) == NULL)
    {
        (void)fprintf(stderr, "%s: could not get environment variable '%s'\n",
                      getprogname(),
                      AED_PASS);
        exit(EXIT_FAILURE);
    }

    if (givenSalt == NULL)
    {
        if (!RAND_bytes(salt, 8))
        {
            (void)fprintf(stderr, "%s: RAND_bytes failed: %s\n",
                          getprogname(),
                          ERR_error_string(ERR_get_error(), NULL));
            exit(EXIT_FAILURE);
        }
        ret.salt = salt;
    }
    else
        ret.salt = givenSalt;

    if (!EVP_BytesToKey(EVP_aes_256_cbc(),
                        EVP_sha1(),
                        salt,
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
    ret.key = key;
    ret.iv = iv;

    return ret;
}
