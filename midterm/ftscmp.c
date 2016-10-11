/* Bradford Smith (bsmith8)
 * CS 631 Midterm ftscmp.c
 * 10/11/2016
 */

#include "ls.h"

/* pre: takes in an FTSENT** a and an FTSENT** b
 * post: compares a and b
 * return: an integer less than, equal to or greater than zero (like strcmp(3))
 *
 * Behaves like cmp.c execpt works on FTSENT**'s
 */
int ftscmp(const FTSENT** a, const FTSENT** b)
{
    struct stat* sa;
    struct stat* sb;

    sa = (*a)->fts_statp;
    sb = (*b)->fts_statp;

    if ((S_ISDIR(sa->st_mode) && S_ISDIR(sb->st_mode)) ||
        (!S_ISDIR(sa->st_mode) && !S_ISDIR(sb->st_mode)))
    {
        if (gl_opts.reverse_sorted)
        {
            if (gl_opts.ctime_sorted)
            {
                if (sa->st_ctim.tv_sec != sb->st_ctim.tv_sec)
                    return sa->st_ctim.tv_sec > sb->st_ctim.tv_sec;
                else if (sa->st_ctim.tv_nsec != sb->st_ctim.tv_nsec)
                    return sa->st_ctim.tv_nsec > sb->st_ctim.tv_nsec;
            }
            else if (gl_opts.t_mtime_sorted)
            {
                if (sa->st_mtim.tv_sec != sb->st_mtim.tv_sec)
                    return sa->st_mtim.tv_sec > sb->st_mtim.tv_sec;
                else if (sa->st_mtim.tv_nsec != sb->st_mtim.tv_nsec)
                    return sa->st_mtim.tv_nsec > sb->st_mtim.tv_nsec;
            }
            else if (gl_opts.u_atime_sorted)
            {
                if (sa->st_atim.tv_sec != sb->st_atim.tv_sec)
                    return sa->st_atim.tv_sec > sb->st_atim.tv_sec;
                else if (sa->st_atim.tv_nsec != sb->st_atim.tv_nsec)
                    return sa->st_atim.tv_nsec > sb->st_atim.tv_nsec;
            }
            else if (gl_opts.Size_sorted)
            {
                if (sa->st_size != sb->st_size)
                    return sa->st_size > sb->st_size;
            }

            /* fall back on lexicographical */
            return strcmp((*b)->fts_name, (*a)->fts_name);
        }
        else
        {
            if (gl_opts.ctime_sorted)
            {
                if (sa->st_ctim.tv_sec != sb->st_ctim.tv_sec)
                    return sa->st_ctim.tv_sec < sb->st_ctim.tv_sec;
                else if (sa->st_ctim.tv_nsec != sb->st_ctim.tv_nsec)
                    return sa->st_ctim.tv_nsec < sb->st_ctim.tv_nsec;
            }
            else if (gl_opts.t_mtime_sorted)
            {
                if (sa->st_mtim.tv_sec != sb->st_mtim.tv_sec)
                    return sa->st_mtim.tv_sec < sb->st_mtim.tv_sec;
                else if (sa->st_mtim.tv_nsec != sb->st_mtim.tv_nsec)
                    return sa->st_mtim.tv_nsec < sb->st_mtim.tv_nsec;
            }
            else if (gl_opts.u_atime_sorted)
            {
                if (sa->st_atim.tv_sec != sb->st_atim.tv_sec)
                    return sa->st_atim.tv_sec < sb->st_atim.tv_sec;
                else if (sa->st_atim.tv_nsec != sb->st_atim.tv_nsec)
                    return sa->st_atim.tv_nsec < sb->st_atim.tv_nsec;
            }
            else if (gl_opts.Size_sorted)
            {
                if (sa->st_size != sb->st_size)
                    return sa->st_size < sb->st_size;
            }

            /* fall back on lexicographical */
            return strcmp((*a)->fts_name, (*b)->fts_name);
        }
    }
    else if(S_ISDIR(sa->st_mode))
    {
        /* other flags only get taken into account if a and b are same type */
        return 1;
    }
    else
    {
        return -1;
    }
}
