/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Code taken from systemd src/fundamental/string-util-fundamental.c
 * NOTE: Semantics differ from glibc strverscmp (e.g. handling of ~rc1)
 */

#include <string.h>
#include <linux/ctype.h>
#include <linux/export.h>

static bool is_valid_version_char(char a)
{
        return isdigit(a) || isalpha(a) || a == '~' ||
		a == '-' || a == '^' || a == '.';
}

int strverscmp(const char *a, const char *b)
{
        /* This function is similar to strverscmp(3), but it treats '-' and '.' as separators.
         *
         * The logic is based on rpm's rpmvercmp(), but unlike rpmvercmp(), it distiguishes e.g.
         * '123a' and '123.a', with '123a' being newer.
         *
         * It allows direct comparison of strings which contain both a version and a release; e.g.
         * '247.2-3.1.fc33.x86_64' or '5.11.0-0.rc5.20210128git76c057c84d28.137.fc34'.
         *
         * The input string is split into segments. Each segment is numeric or alphabetic, and may be
         * prefixed with the following:
         *  '~' : used for pre-releases, a segment prefixed with this is the oldest,
         *  '-' : used for the separator between version and release,
         *  '^' : used for patched releases, a segment with this is newer than one with '-'.
         *  '.' : used for point releases.
         * Note that no prefix segment is the newest. All non-supported characters are dropped, and
         * handled as a separator of segments, e.g., '123_a' is equivalent to '123a'.
         *
         * By using this, version strings can be sorted like following:
         *  (older) 122.1
         *     ^    123~rc1-1
         *     |    123
         *     |    123-a
         *     |    123-a.1
         *     |    123-1
         *     |    123-1.1
         *     |    123^post1
         *     |    123.a-1
         *     |    123.1-1
         *     v    123a-1
         *  (newer) 124-1
         */

        a = a ?: "";
        b = b ?: "";

        for (;;) {
                const char *aa, *bb;
                int r;

                /* Drop leading invalid characters. */
                while (*a != '\0' && !is_valid_version_char(*a))
                        a++;
                while (*b != '\0' && !is_valid_version_char(*b))
                        b++;

                /* Handle '~'. Used for pre-releases, e.g. 123~rc1, or 4.5~alpha1 */
                if (*a == '~' || *b == '~') {
                        /* The string prefixed with '~' is older. */
                        r = compare3(*a != '~', *b != '~');
                        if (r != 0)
                                return r;

                        /* Now both strings are prefixed with '~'. Compare remaining strings. */
                        a++;
                        b++;
                }

                /* If at least one string reaches the end, then longer is newer.
                 * Note that except for '~' prefixed segments, a string which has more segments is newer.
                 * So, this check must be after the '~' check. */
                if (*a == '\0' || *b == '\0')
                        return compare3(*a, *b);

                /* Handle '-', which separates version and release, e.g 123.4-3.1.fc33.x86_64 */
                if (*a == '-' || *b == '-') {
                        /* The string prefixed with '-' is older (e.g., 123-9 vs 123.1-1) */
                        r = compare3(*a != '-', *b != '-');
                        if (r != 0)
                                return r;

                        a++;
                        b++;
                }

                /* Handle '^'. Used for patched release. */
                if (*a == '^' || *b == '^') {
                        r = compare3(*a != '^', *b != '^');
                        if (r != 0)
                                return r;

                        a++;
                        b++;
                }

                /* Handle '.'. Used for point releases. */
                if (*a == '.' || *b == '.') {
                        r = compare3(*a != '.', *b != '.');
                        if (r != 0)
                                return r;

                        a++;
                        b++;
                }

                if (isdigit(*a) || isdigit(*b)) {
                        /* Find the leading numeric segments. One may be an empty string. So,
                         * numeric segments are always newer than alpha segments. */
                        for (aa = a; isdigit(*aa); aa++)
                                ;
                        for (bb = b; isdigit(*bb); bb++)
                                ;

                        /* Check if one of the strings was empty, but the other not. */
                        r = compare3(a != aa, b != bb);
                        if (r != 0)
                                return r;

                        /* Skip leading '0', to make 00123 equivalent to 123. */
                        while (*a == '0')
                                a++;
                        while (*b == '0')
                                b++;

                        /* To compare numeric segments without parsing their values, first compare the
                         * lengths of the segments. Eg. 12345 vs 123, longer is newer. */
                        r = compare3(aa - a, bb - b);
                        if (r != 0)
                                return r;

                        /* Then, compare them as strings. */
                        r = compare3(strncmp(a, b, aa - a), 0);
                        if (r != 0)
                                return r;
                } else {
                        /* Find the leading non-numeric segments. */
                        for (aa = a; isalpha(*aa); aa++)
                                ;
                        for (bb = b; isalpha(*bb); bb++)
                                ;

                        /* Note that the segments are usually not NUL-terminated. */
                        r = compare3(strncmp(a, b, min(aa - a, bb - b)), 0);
                        if (r != 0)
                                return r;

                        /* Longer is newer, e.g. abc vs abcde. */
                        r = compare3(aa - a, bb - b);
                        if (r != 0)
                                return r;
                }

                /* The current segments are equivalent. Let's move to the next one. */
                a = aa;
                b = bb;
        }
}
EXPORT_SYMBOL(strverscmp);
