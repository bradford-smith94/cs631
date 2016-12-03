/* Bradford Smith (bsmith8)
 * CS 631 HW 4 aed.h
 * 12/03/2016
 */

#ifndef _AED_H_
#define _AED_H_

#define AED_PASS "AED_PASS"
#define ENC_PREFIX "Salted__"

struct s_privates
{
    unsigned char* key;
    unsigned char* iv;
    unsigned char* salt;
};
typedef struct s_privates t_privates;

int main(int, char**);

t_privates genkey(unsigned char*);

void decrypt();
void encrypt();
void usage();

#endif /* _AED_H_ */
