/* Bradford Smith (bsmith8)
 * CS 631 HW 4 aed.c
 * 12/04/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>

#include <openssl/err.h>
#include <openssl/evp.h>

#include <stdlib.h>
#include <unistd.h>

#define HELPSTR ""\
"\t-d\tPerform decryption on stdin.\n"\
"\t-e\tPerform encryption on stdin.\n"\
"\t-h\tPrint this help.\n"

int main(int argc, char** argv)
{
    int opt;

    setprogname(argv[0]);
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    while ((opt = getopt(argc, argv, "deh")) != -1)
    {
        switch (opt)
        {
            case 'd':
                decrypt();
                break;
            case 'e':
                encrypt();
                break;
            case 'h':
                usage();
                (void)fprintf(stderr, HELPSTR);
                return EXIT_SUCCESS;
            default:
                usage();
                return EXIT_FAILURE;
        }
    }

    if (optind == 1) /* we have no options */
    {
        usage();
        return EXIT_FAILURE;
    }

    ERR_free_strings();

    return EXIT_SUCCESS;
}
