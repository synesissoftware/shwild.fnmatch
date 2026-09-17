/* /////////////////////////////////////////////////////////////////////////
 * File:    shwild.fnmatch/shwild.fnmatch.h
 *
 * Purpose: fnmatch() implementation based on shwild.
 *
 * Created: 8th May 2006
 * Updated: 18th September 2026
 *
 * Home:    https://github.com/synesissoftware/shwild.fnmatch
 *
 * Copyright (c) 2019-2026, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2006-2019, Matthew Wilson and Synesis Software
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


/** \file shwild.fnmatch/shwild.fnmatch.h [C/C++] Root header of the
 *   **shwild.fnmatch** C API.
 */

#ifndef SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_H_SHWILD_FNMATCH
#define SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_H_SHWILD_FNMATCH


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#ifndef SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_H_SHWILD_FNMATCH_MAJOR       1
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_H_SHWILD_FNMATCH_MINOR       1
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_H_SHWILD_FNMATCH_REVISION    0
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_H_SHWILD_FNMATCH_EDIT        6
#endif /* !SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION */

/** \def SHWILD_FNMATCH_VER_MAJOR
 * The major version number of **shwild.fnmatch**.
 */

/** \def SHWILD_FNMATCH_VER_MINOR
 * The minor version number of **shwild.fnmatch**.
 */

/** \def SHWILD_FNMATCH_VER_PATCH
 * The patch version number of **shwild.fnmatch**.
 */

/** \def SHWILD_FNMATCH_VER
 * The current composite version number of **shwild.fnmatch**.
 */

#define SHWILD_FNMATCH_VER_MAJOR       0
#define SHWILD_FNMATCH_VER_MINOR       9
#define SHWILD_FNMATCH_VER_PATCH       0
#define SHWILD_FNMATCH_VER_ALPHABETA   0xC1

#define SHWILD_FNMATCH_VER \
    (0\
        |   (   SHWILD_FNMATCH_VER_MAJOR       << 24   ) \
        |   (   SHWILD_FNMATCH_VER_MINOR       << 16   ) \
        |   (   SHWILD_FNMATCH_VER_PATCH       <<  8   ) \
        |   (   SHWILD_FNMATCH_VER_ALPHABETA   <<  0   ) \
    )

#ifndef SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION
# define SHWILD_FNMATCH_VER_REVISION                        SHWILD_FNMATCH_VER_PATCH
#endif /* !SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * documentation
 */

/** \defgroup group__shwild_fnmatch_api shwild.fnmatch API
 * \brief The shwild.fnmatch public API
 *
 * The types, constants and functions that constitute the shwild.fnmatch
 * API. Users of the library use these functions to effect file-name
 * matching.
 */

/** \defgroup group__shwild_fnmatch_api__c_api C API
 * \ingroup group__shwild_fnmatch_api
 * \brief These functions form the core of the shwild.fnmatch API
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * constants and definitions
 */

/** \defgroup group__shwild_fnmatch_api__flags Pattern Control Flags
 * \ingroup group__shwild_fnmatch_api
 * \brief These flags control the pattern matching behaviour
 *
 * @{
 */

#define SHWILD_FNMATCH_RC_SUCCESS       (0)         /*!< \brief String matched the pattern. */
#define SHWILD_FNMATCH_RC_ERROR         (-1)        /*!< \brief An error occurred. */
#define SHWILD_FNMATCH_RC_NOMATCH       (1)         /*!< \brief String did not match the pattern. */

#define SHWILD_FNMATCH_F_NOESCAPE       (0x0001)    /*!< \brief Treat \\ as literals. When not specified, \\ escapes the next character. On operating systems that recognise backslash as a path name separator this is assumed, unless SHWILD_FNMATCH_F_NONOESCAPE is specified. */
#define SHWILD_FNMATCH_F_PATHNAME       (0x0002)    /*!< \brief / in string do not match wildcards. Pattern must contain a corresponding number of /. */
#define SHWILD_FNMATCH_F_PERIOD         (0x0004)    /*!< \brief Leading . (or any . following / with SHWILD_FNMATCH_F_PATHNAME) in string does not match wildcards. Pattern must contain corresponding element(s). */
#define SHWILD_FNMATCH_F_IGNORECASE     (0x0008)    /*!< \brief Ignores case. On operating systems that do case-insensitive comparison this is assumed, unless SHWILD_FNMATCH_F_NOIGNORECASE is specified. Explicit setting of this flag overrides SHWILD_FNMATCH_F_NOIGNORECASE. */
#define SHWILD_FNMATCH_F_NOIGNORECASE   (0x0010)    /*!< \brief Suppresses assumption of SHWILD_FNMATCH_F_IGNORECASE on operating systems that do case-insensitive comparison. Ignored on other systems. */
#define SHWILD_FNMATCH_F_NONOESCAPE     (0x0020)    /*!< \brief On operating systems that recognise backslash as a path name separator SHWILD_FNMATCH_F_NOESCAPE is assumed, unless this is specified. Ignored on other systems. */
#define SHWILD_FNMATCH_F_NOSLASHSWAP    (0x0040)    /*!< \brief On operating systems that recognise backslash as a path name separator, pattern and strings are canonicalised by swapping \\ to / when SHWILD_FNMATCH_F_PATHNAME is specified, unless this flag is specified. */
#define SHWILD_FNMATCH_F_NOIMPLICITROOT (0x0080)    /*!< \brief On operating systems that have drives/volumes, leading / (or \\) in the pattern are treated as matching any root, including drive volumes (e.g. H:\\) and UNC path roots, when SHWILD_FNMATCH_F_PATHNAME is specified, unless this flag is specified. */

/** @} */


/* /////////////////////////////////////////////////////////////////////////
 * typedefs
 */

/** \brief Handle type for use in declaring opaque handles to compiled patterns
 * \ingroup group__shwild_fnmatch_api__c_api
 */
struct shwild_fnmatch_handle_t_;

/** \brief Handle to compiled pattern.
 * \ingroup group__shwild_fnmatch_api__c_api
 *
 * Used by shwild_fnmatch_compile_pattern(), shwild_fnmatch_match_pattern()
 * and shwild_fnmatch_destroy_pattern()
 */
typedef struct shwild_fnmatch_handle_t_ *shwild_fnmatch_handle_t;


/* /////////////////////////////////////////////////////////////////////////
 * functions
 */

/** \brief Matches a string against a pattern
 * \ingroup group__shwild_fnmatch_api__c_api
 *
 * \param pattern The pattern against which matching will be performed
 * \param string The string to match against the pattern
 * \param flags Flags which moderate the search.
 *
 * \return Status indicating whether the string matched against the given pattern
 * \retval SHWILD_FNMATCH_RC_SUCCESS The string matched the pattern
 * \retval SHWILD_FNMATCH_RC_NOMATCH The string did not match the pattern
 * \retval <0 An error code
 *
 * \pre NULL != pattern
 * \pre NULL != string
 */
int
shwild_fnmatch_match(
    char const* pattern
,   char const* string
,   unsigned    flags
);

/** \brief Compiles a pattern into an efficient form for use in multiple match operations
 * \ingroup group__shwild_fnmatch_api__c_api
 *
 * \param pattern The fnmatch pattern against which matching will be performed
 * \param flags Flags which moderate the search
 * \param phCompiledPattern Pointer to a variable to hold the compiled pattern
 *
 * \return Status indicating whether the operation completed successfully
 * \retval <0 The operation failed
 * \retval >=0 The operation succeeded. The value indicates the number of match
 *   sub-components created to represent the pattern. The compiled pattern must
 *   be destroyed when it is no longer needed, by shwild_fnmatch_destroy_pattern(),
 *   to avoid memory leaks.
 *
 * \pre NULL != pattern
 * \pre NULL != phCompiledPattern
 *
 * \see shwild_fnmatch_match_pattern
 * \see shwild_fnmatch_destroy_pattern
 */
int
shwild_fnmatch_compile_pattern(
    char const*                 pattern
,   unsigned                    flags
,   shwild_fnmatch_handle_t*    phCompiledPattern
);

/** \brief Matches a string against a pre-compiled shwild.fnmatch pattern
 * \ingroup group__shwild_fnmatch_api__c_api
 *
 * \param hCompiledPattern The precompiled fnmatch pattern against which matching will be performed
 * \param string The string to match against the pattern
 *
 * \return Status indicating whether the string matched against the given pattern
 * \retval SHWILD_FNMATCH_RC_SUCCESS The string matched the pattern
 * \retval SHWILD_FNMATCH_RC_NOMATCH The string did not match the pattern
 * \retval <0 An error code
 *
 * \pre NULL != hCompiledPattern
 * \pre NULL != string
 *
 * \see shwild_fnmatch_compile_pattern
 * \see shwild_fnmatch_destroy_pattern
 */
int
shwild_fnmatch_match_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
,   char const*             string
);

/** \brief Releases all resources associated with a pre-compiled shwild.fnmatch pattern
 * \ingroup group__shwild_fnmatch_api__c_api
 *
 * \param hCompiledPattern The pattern to be destroyed.
 *
 * \see shwild_fnmatch_compile_pattern
 * \see shwild_fnmatch_match_pattern
 */
void
shwild_fnmatch_destroy_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
);


/* ////////////////////////////////////////////////////////////////////// */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(__cplusplus) && \
    !defined(SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION) && \
    !defined(SHWILD_FNMATCH_NO_NAMESPACE)
# define SHWILD_FNMATCH_NO_NAMESPACE
#endif /* __cplusplus, etc. */

#if !defined(SHWILD_FNMATCH_NO_NAMESPACE)
namespace shwild
{
/** \brief The shwild.fnmatch/C++ namespace - <code>shwild::fnmatch</code> - that contains wrappers for the
 * \link group__shwild_fnmatch_api__c_api C API\endlink. */
namespace fnmatch
{
#endif /* !SHWILD_FNMATCH_NO_NAMESPACE */


/* /////////////////////////////////////////////////////////////////////////
 * C++ functions
 */

#ifdef __cplusplus

/** \defgroup group__shwild_fnmatch_api__cpp_api C++ API
 * \ingroup group__shwild_fnmatch_api
 * \brief The C++ API provides convenient overloads of the
 *   \ref group__shwild_fnmatch_api__c_api functions for use in C++, along with the
 *   Pattern class that provides a convenient interface to compiled patterns.
 * @{
 */

/** \brief C++ overload synonym for shwild_fnmatch_match()
 * \ingroup group__shwild_fnmatch_api__cpp_api
 */
inline
int
match(
    char const* pattern
,   char const* string
,   unsigned    flags = 0
)
{
    return shwild_fnmatch_match(pattern, string, flags);
}

/** \brief C++ overload synonym for shwild_fnmatch_compile_pattern()
 * \ingroup group__shwild_fnmatch_api__cpp_api
 */
inline
int
compile_pattern(
    char const*                 pattern
,   unsigned                    flags
,   shwild_fnmatch_handle_t*    phCompiledPattern
)
{
    return shwild_fnmatch_compile_pattern(pattern, flags, phCompiledPattern);
}

/** \brief C++ overload synonym for shwild_fnmatch_match_pattern()
 * \ingroup group__shwild_fnmatch_api__cpp_api
 */
inline
int
match_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
,   char const*             string
)
{
    return shwild_fnmatch_match_pattern(hCompiledPattern, string);
}

/** \brief C++ overload synonym for shwild_fnmatch_destroy_pattern()
 * \ingroup group__shwild_fnmatch_api__cpp_api
 */
inline
void
destroy_pattern(
    shwild_fnmatch_handle_t hCompiledPattern
)
{
    shwild_fnmatch_destroy_pattern(hCompiledPattern);
}

/** @} */

#endif /* __cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(SHWILD_FNMATCH_NO_NAMESPACE)
} /* namespace fnmatch */
} /* namespace shwild */
#endif /* !SHWILD_FNMATCH_NO_NAMESPACE */


/* ////////////////////////////////////////////////////////////////////// */

#endif /* SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_H_SHWILD_FNMATCH */

/* ///////////////////////////// end of file //////////////////////////// */
