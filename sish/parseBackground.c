/* Bradford Smith (bsmith8)
 * CS 631 sish parseBackground.c
 * 12/17/2016
 */

#include "sish.h"

#include <stdlib.h>
#include <string.h>

/* pre: takes in a char** 'command' representing a command string as a char*
 *      array
 * post: searches for and removes the background control character from the
 *       command string if it is present as it's own word or at the end of the
 *       last word
 * return: an int, >0 if this command is to be backgrounded, else 0
 */
int parseBackground(char** command)
{
    int bg_len;
    int c_len;
    int ret;
    int words;

    ret = 0;
    bg_len = strlen(BG_STR);

    /* count words in command, note semicolon */
    for (words = 0; command[words] != NULL; words++);

    /* length of last word */
    c_len = strlen(command[words - 1]);

    if (!strncmp(command[words - 1], BG_STR, bg_len))
    {
        ret = 1;
        removeIndex(command, words - 1);
    }
    else if (!strncmp(&command[words - 1][c_len - bg_len], BG_STR, bg_len))
    {
        ret = 1;
        command[words - 1][c_len - bg_len] = '\0';
    }

    return ret;
}
