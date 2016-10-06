/* Bradford Smith
 * CS 631 Midterm swap.c
 * 10/06/2016
 */

#include "ls.h"

/* pre: none, takes in a char** 'a' and a char** 'b'
 * post: preforms a swap of 'a' and 'b'
 */
void swap(char** a, char** b)
{
    char* temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
