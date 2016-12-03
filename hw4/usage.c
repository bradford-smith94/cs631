/* Bradford Smith
 * CS 631 HW 4 usage.c
 * 12/03/2016
 */

#include "aed.h"

#include <bsd/stdlib.h>
#include <stdio.h>
#include <stdlib.h>

void usage()
{
    (void)fprintf(stderr, "usage: ./%s: [-deh]\n", getprogname());
}
