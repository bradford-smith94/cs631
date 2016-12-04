/* Bradford Smith (bsmith8)
 * CS 631 HW 4 aed.h
 * 12/03/2016
 */

#ifndef _AED_H_
#define _AED_H_

#include <openssl/evp.h>

#define AED_PASS "AED_PASS"
#define ENC_PREFIX "Salted__"
#define SALT_SIZE 8

struct s_privates
{
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char salt[SALT_SIZE];
};
typedef struct s_privates t_privates;

int main(int, char**);

t_privates* genkey(unsigned char*);

void decrypt();
void encrypt();
void usage();

#endif /* _AED_H_ */
