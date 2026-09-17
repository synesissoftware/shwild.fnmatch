/* /////////////////////////////////////////////////////////////////////////
 * File:    src/shwild.fnmatch.c
 *
 * Purpose: Implementation of the shwild.fnmatch API.
 *
 * Created: 8th May 2006
 * Updated: 17th September 2026
 *
 * Home:    https://github.com/synesissoftware/shwild.fnmatch
 *
 * Copyright (c) 2019-2026, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2006-2019, Matthew Wilson and Sean Kelly
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


/** \file src/shwild.fnmatch.c Purpose: Implementation of the
 *   shwild.fnmatch API; INTERNAL FILE.
 */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <shwild.fnmatch/shwild.fnmatch.h>

#include <shwild/shwild.h>
#include <cstring/cstring.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "shwild.fnmatch.assert.h"


/* /////////////////////////////////////////////////////////////////////////
 * platform discrimination
 */

#if defined(WIN32) || \
    defined(_WIN32) || \
    defined(WIN64) || \
    defined(_WIN64)
# define SHWILD_FNMATCH_OS_IS_CASE_INSENSITIVE_
# define SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
# define SHWILD_FNMATCH_OS_USES_DRIVES_
# define SHWILD_FNMATCH_OS_USES_UNC_PATHS_
#endif /* operating system */


/* /////////////////////////////////////////////////////////////////////////
 * typedefs
 */

/** \brief Context structure. */
struct shwild_fnmatch_handle_t_
{
    shwild_handle_t     hShWildCompiledPattern; /*!< shwild compiled-pattern handle */
    unsigned            flags;                  /*!< shwild.fnmatch flags           */
    cstring_t           pattern;                /*!< search pattern                 */
};


/* /////////////////////////////////////////////////////////////////////////
 * utility functions
 */

static
size_t
strnumchr(
    char const* s
,   char        ch
)
{
    size_t i;

    for (i = 0; '\0' != *s; ++s)
    {
        if (*s == ch)
        {
            ++i;
        }
    }

    return i;
}

#ifdef SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
static
void
strnrepl(
    char*   s
,   size_t  n
,   char    chFrom
,   char    chTo
)
{
    char*       begin = s;
    char const* end   = s + n;

    SHWILD_FNMATCH_ASSERT(NULL != s);

    for (; begin != end; ++begin)
    {
        if (chFrom == *begin)
        {
            *begin = chTo;
        }
    }
}

static
int
canonicalise_win32_path_(
    cstring_t* cs
)
{
    char*       begin = cs->ptr;
    char const* end   = begin + cs->len;
    char*       dest  = cs->ptr;
    size_t      numElided = 0;

    SHWILD_FNMATCH_ASSERT(NULL != cs);
    SHWILD_FNMATCH_ASSERT(NULL != cs->ptr);

    for (; begin != end; ++begin)
    {
        if (dest != begin)
        {
            *dest = *begin;
        }

        if ('\\' == begin[0] &&
            '\\' == begin[1])
        {
            begin[1] = '/';
            ++numElided;
        }
        else
        {
            ++dest;
        }
    }

    SHWILD_FNMATCH_ASSERT(cs->len >= numElided);
    cs->len -= numElided;
    cs->ptr[cs->len] = '\0';

    strnrepl(cs->ptr, cs->len, '\\', '/');

    return 0;
}
#endif /* SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_ */

static
unsigned
shwild_fnmatch_flags_to_shwild_flags_(
    unsigned flags
)
{
    unsigned shwild_flags = 0;

#ifdef SHWILD_FNMATCH_OS_IS_CASE_INSENSITIVE_
    if (0 == (SHWILD_FNMATCH_F_NOIGNORECASE & flags))
    {
        shwild_flags |= SHWILD_F_IGNORE_CASE;
    }
#endif /* SHWILD_FNMATCH_OS_IS_CASE_INSENSITIVE_ */

#ifdef SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
    if (0 != (SHWILD_FNMATCH_F_NONOESCAPE & flags))
    {
        shwild_flags |= SHWILD_F_SUPPRESS_BACKSLASH_ESCAPE;
    }
#endif /* SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_ */

    if (SHWILD_FNMATCH_F_IGNORECASE & flags)
    {
        shwild_flags |= SHWILD_F_IGNORE_CASE;
    }
    if (SHWILD_FNMATCH_F_NOESCAPE & flags)
    {
        shwild_flags |= SHWILD_F_SUPPRESS_BACKSLASH_ESCAPE;
    }

    return shwild_flags;
}

static
int
shwild_fnmatch_prematch_(
    char const*     pattern
,   char const**    string
,   unsigned        flags
)
{
    SHWILD_FNMATCH_ASSERT(NULL != pattern);
    SHWILD_FNMATCH_ASSERT(NULL != string);
    SHWILD_FNMATCH_ASSERT(NULL != *string);

    if (SHWILD_FNMATCH_F_PATHNAME == ((SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_NOIMPLICITROOT) & flags))
    {
        if ('/' == pattern[0] ||
            '\\' == pattern[0])
        {
#ifdef SHWILD_FNMATCH_OS_USES_DRIVES_
            if (isalpha((unsigned char)(*string)[0]) &&
                ':' == (*string)[1] &&
                ('/' == (*string)[2] ||
                 '\\' == (*string)[2]))
            {
                *string += 2;

                return shwild_fnmatch_prematch_(pattern, string, flags | SHWILD_FNMATCH_F_NOIMPLICITROOT);
            }
#endif /* SHWILD_FNMATCH_OS_USES_DRIVES_ */
#ifdef SHWILD_FNMATCH_OS_USES_UNC_PATHS_
            if ('\\' == (*string)[0] &&
                '\\' == (*string)[1])
            {
                *string += 2;
                *string = strchr(*string, '\\');
                if (NULL == *string)
                {
                    return SHWILD_FNMATCH_RC_NOMATCH;
                }
                *string += 1;
                *string = strchr(*string, '\\');
                if (NULL == *string)
                {
                    return SHWILD_FNMATCH_RC_NOMATCH;
                }

                return shwild_fnmatch_prematch_(pattern, string, flags | SHWILD_FNMATCH_F_NOIMPLICITROOT);
            }
#endif /* SHWILD_FNMATCH_OS_USES_UNC_PATHS_ */
        }
    }

    switch ((SHWILD_FNMATCH_F_PERIOD | SHWILD_FNMATCH_F_PATHNAME) & flags)
    {
        case 0:
            break;
        case SHWILD_FNMATCH_F_PERIOD:
            if ('.' == (*string)[0] &&
                '.' != pattern[0])
            {
                return SHWILD_FNMATCH_RC_NOMATCH;
            }
            break;
        case SHWILD_FNMATCH_F_PATHNAME:
        case SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD:
            if (strnumchr(pattern, '/') != strnumchr(*string, '/'))
            {
                return SHWILD_FNMATCH_RC_NOMATCH;
            }
            if (SHWILD_FNMATCH_F_PERIOD & flags)
            {
                if ('.' == (*string)[0] &&
                    '.' != pattern[0])
                {
                    return SHWILD_FNMATCH_RC_NOMATCH;
                }
                else
                {
                    char const* s1 = *string;
                    char const* s2 = pattern;

                    for (;; ++s1, ++s2)
                    {
                        s1 = strchr(s1, '/');
                        s2 = strchr(s2, '/');

                        if (NULL == s1 ||
                            NULL == s2)
                        {
                            break;
                        }
                        else
                        {
                            if ('.' == s1[1] &&
                                '.' != s2[1])
                            {
                                break;
                            }
                        }
                    }

                    if (s1 != s2)
                    {
                        return SHWILD_FNMATCH_RC_NOMATCH;
                    }
                }
            }
            break;
    }

    return SHWILD_FNMATCH_RC_SUCCESS;
}


/* ////////////////////////////////////////////////////////////////////// */

int
shwild_fnmatch_match(
    char const* pattern
,   char const* string
,   unsigned    flags
)
{
    SHWILD_FNMATCH_ASSERT(NULL != pattern);
    SHWILD_FNMATCH_ASSERT(NULL != string);

#ifdef SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
    if (SHWILD_FNMATCH_F_PATHNAME == ((SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_NOSLASHSWAP) & flags))
    {
        int         res;
        cstring_t   csPattern;
        cstring_t   csString;

        cstring_init(&csPattern);
        cstring_init(&csString);

        if (CSTRING_RC_SUCCESS != cstring_create(&csPattern, pattern) ||
            CSTRING_RC_SUCCESS != cstring_create(&csString, string))
        {
            res = SHWILD_FNMATCH_RC_ERROR;
        }
        else
        {
            canonicalise_win32_path_(&csPattern);
            canonicalise_win32_path_(&csString);

            res = shwild_fnmatch_match(csPattern.ptr, csString.ptr, flags | SHWILD_FNMATCH_F_NOSLASHSWAP);
        }

        cstring_destroy(&csPattern);
        cstring_destroy(&csString);

        return res;
    }
#endif /* SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_ */

    {
        unsigned    shwild_flags    =   shwild_fnmatch_flags_to_shwild_flags_(flags);
        int         res             =   shwild_fnmatch_prematch_(pattern, &string, flags);

        if (SHWILD_FNMATCH_RC_SUCCESS != res)
        {
            return res;
        }

        return shwild_match(pattern, string, shwild_flags);
    }
}

int
shwild_fnmatch_compile_pattern(
    char const*                 pattern
,   unsigned                    flags
,   shwild_fnmatch_handle_t*    phCompiledPattern
)
{
    SHWILD_FNMATCH_ASSERT(NULL != pattern);
    SHWILD_FNMATCH_ASSERT(NULL != phCompiledPattern);

#ifdef SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
    if (SHWILD_FNMATCH_F_PATHNAME == ((SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_NOSLASHSWAP) & flags))
    {
        int         res;
        cstring_t   csPattern;

        cstring_init(&csPattern);

        if (CSTRING_RC_SUCCESS != cstring_create(&csPattern, pattern))
        {
            res = SHWILD_FNMATCH_RC_ERROR;
        }
        else
        {
            canonicalise_win32_path_(&csPattern);

            res = shwild_fnmatch_compile_pattern(csPattern.ptr, flags | SHWILD_FNMATCH_F_NOSLASHSWAP, phCompiledPattern);

            if (res >= 0 &&
                NULL != *phCompiledPattern)
            {
                /* Retain the caller's flags so match_pattern() can
                 * canonicalise the subject string in the same way.
                 */
                (*phCompiledPattern)->flags = flags;
            }
        }

        cstring_destroy(&csPattern);

        return res;
    }
#endif /* SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_ */

    {
        shwild_handle_t hShWildCompiledPattern;
        int             shwild_res;
        unsigned        shwild_flags    =   shwild_fnmatch_flags_to_shwild_flags_(flags);
        cstring_t       patternCopy;
        CSTRING_RC      rc;

        *phCompiledPattern = NULL;

        rc = cstring_create(&patternCopy, pattern);

        if (CSTRING_RC_OUTOFMEMORY == rc)
        {
            shwild_res = SHWILD_RC_ALLOC_ERROR;
        }
        else if (CSTRING_RC_SUCCESS != rc)
        {
            shwild_res = SHWILD_FNMATCH_RC_ERROR;
        }
        else
        {
            shwild_res = shwild_compile_pattern(pattern, shwild_flags, &hShWildCompiledPattern);

            if (shwild_res >= 0)
            {
                *phCompiledPattern = malloc(sizeof(struct shwild_fnmatch_handle_t_));

                if (NULL == *phCompiledPattern)
                {
                    shwild_destroy_pattern(hShWildCompiledPattern);

                    shwild_res = SHWILD_RC_ALLOC_ERROR;
                }
                else
                {
                    (*phCompiledPattern)->hShWildCompiledPattern    =   hShWildCompiledPattern;
                    (*phCompiledPattern)->flags                     =   flags;
                    cstring_init(&(*phCompiledPattern)->pattern);
                    cstring_swap(&(*phCompiledPattern)->pattern, &patternCopy);
                }
            }

            cstring_destroy(&patternCopy);
        }

        return shwild_res;
    }
}

int
shwild_fnmatch_match_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
,   char const*             string
)
{
    struct shwild_fnmatch_handle_t_*    handle;
    int                                 res;

    SHWILD_FNMATCH_ASSERT(NULL != hCompiledPattern);
    SHWILD_FNMATCH_ASSERT(NULL != string);

    handle = (struct shwild_fnmatch_handle_t_*)hCompiledPattern;

#ifdef SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_
    if (SHWILD_FNMATCH_F_PATHNAME == ((SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_NOSLASHSWAP) & handle->flags))
    {
        cstring_t csString;

        cstring_init(&csString);

        if (CSTRING_RC_SUCCESS != cstring_create(&csString, string))
        {
            return SHWILD_FNMATCH_RC_ERROR;
        }

        canonicalise_win32_path_(&csString);

        {
            char const* subject = csString.ptr;

            res = shwild_fnmatch_prematch_(handle->pattern.ptr, &subject, handle->flags | SHWILD_FNMATCH_F_NOSLASHSWAP);

            if (SHWILD_FNMATCH_RC_SUCCESS == res)
            {
                res = shwild_match_pattern(handle->hShWildCompiledPattern, subject);
            }
        }

        cstring_destroy(&csString);

        return res;
    }
#endif /* SHWILD_FNMATCH_OS_USES_BACKSLASH_SEPARATOR_ */

    res = shwild_fnmatch_prematch_(handle->pattern.ptr, &string, handle->flags);

    if (SHWILD_FNMATCH_RC_SUCCESS != res)
    {
        return res;
    }

    return shwild_match_pattern(handle->hShWildCompiledPattern, string);
}

void
shwild_fnmatch_destroy_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
)
{
    struct shwild_fnmatch_handle_t_* handle;

    if (NULL == hCompiledPattern)
    {
        return;
    }

    handle = (struct shwild_fnmatch_handle_t_*)hCompiledPattern;

    shwild_destroy_pattern(handle->hShWildCompiledPattern);
    cstring_destroy(&handle->pattern);
    free(handle);
}


/* ///////////////////////////// end of file //////////////////////////// */
