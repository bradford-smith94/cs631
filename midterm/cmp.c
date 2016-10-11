/* Bradford Smith (bsmith8)
 * CS 631 Midterm cmp.c
 * 10/11/2016
 */

#include "ls.h"

/* pre: takes in a char* a, and a char* b which are not NULL
 * post: compares a and b
 * return: an integer less than, equal to, or greater than zero (like strcmp(3))
 *
 * Behaves like strcmp(3) except also takes into account whether or not a or b
 * represent a directory as well as the option flags regarding sorting
 */
int cmp(char* a, char* b)
{
    struct stat sa;
    struct stat sb;

    if (stat(a, &sa) < 0)
    {
        fprintf(stderr, "%s: could not stat '%s': %s\n",
                gl_progname,
                a,
                strerror(errno));
        exit(1);
    }

    if (stat(b, &sb) < 0)
    {
        fprintf(stderr, "%s: could not stat '%s': %s\n",
                gl_progname,
                b,
                strerror(errno));
        exit(1);
    }

    if ((S_ISDIR(sa.st_mode) && S_ISDIR(sb.st_mode)) ||
        (!S_ISDIR(sa.st_mode) && !S_ISDIR(sb.st_mode)))
    {
        if (gl_opts.reverse_sorted)
        {
            if (gl_opts.ctime_sorted)
            {
                if (sa.st_ctim.tv_sec != sb.st_ctim.tv_sec)
                    return sa.st_ctim.tv_sec > sb.st_ctim.tv_sec;
                else if (sa.st_ctim.tv_nsec != sb.st_ctim.tv_nsec)
                    return sa.st_ctim.tv_nsec > sb.st_ctim.tv_nsec;
            }
            else if (gl_opts.t_mtime_sorted)
            {
                if (sa.st_mtim.tv_sec != sb.st_mtim.tv_sec)
                    return sa.st_mtim.tv_sec > sb.st_mtim.tv_sec;
                else if (sa.st_mtim.tv_nsec != sb.st_mtim.tv_nsec)
                    return sa.st_mtim.tv_nsec > sb.st_mtim.tv_nsec;
            }
            else if (gl_opts.u_atime_sorted)
            {
                if (sa.st_atim.tv_sec != sb.st_atim.tv_sec)
                    return sa.st_atim.tv_sec > sb.st_atim.tv_sec;
                else if (sa.st_atim.tv_nsec != sb.st_atim.tv_nsec)
                    return sa.st_atim.tv_nsec > sb.st_atim.tv_nsec;
            }
            else if (gl_opts.Size_sorted)
            {
                if (sa.st_size != sb.st_size)
                    return sa.st_size > sb.st_size;
            }

            /* fall back on lexicographical */
            return strcmp(b, a);
        }
        else
        {
            if (gl_opts.ctime_sorted)
            {
                if (sa.st_ctim.tv_sec != sb.st_ctim.tv_sec)
                    return sa.st_ctim.tv_sec < sb.st_ctim.tv_sec;
                else if (sa.st_ctim.tv_nsec != sb.st_ctim.tv_nsec)
                    return sa.st_ctim.tv_nsec < sb.st_ctim.tv_nsec;
            }
            else if (gl_opts.t_mtime_sorted)
            {
                if (sa.st_mtim.tv_sec != sb.st_mtim.tv_sec)
                    return sa.st_mtim.tv_sec < sb.st_mtim.tv_sec;
                else if (sa.st_mtim.tv_nsec != sb.st_mtim.tv_nsec)
                    return sa.st_mtim.tv_nsec < sb.st_mtim.tv_nsec;
            }
            else if (gl_opts.u_atime_sorted)
            {
                if (sa.st_atim.tv_sec != sb.st_atim.tv_sec)
                    return sa.st_atim.tv_sec < sb.st_atim.tv_sec;
                else if (sa.st_atim.tv_nsec != sb.st_atim.tv_nsec)
                    return sa.st_atim.tv_nsec < sb.st_atim.tv_nsec;
            }
            else if (gl_opts.Size_sorted)
            {
                if (sa.st_size != sb.st_size)
                    return sa.st_size < sb.st_size;
            }

            /* fall back on lexicographical */
            return strcmp(a, b);
        }
    }
    else if(S_ISDIR(sa.st_mode))
    {
        /* other flags only get taken into account if a and b are same type */
        return 1;
    }
    else
    {
        return -1;
    }
}
