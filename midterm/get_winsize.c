/* Bradford Smith
 * CS 631 Midterm get_winsize.c
 * 10/07/2016
 */

#include "ls.h"

#include <sys/ioctl.h>

struct winsize get_win_size()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w;
}
