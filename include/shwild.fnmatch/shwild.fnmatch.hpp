/* /////////////////////////////////////////////////////////////////////////
 * File:    shwild.fnmatch/shwild.fnmatch.hpp
 *
 * Purpose: C++ root file for the shwild.fnmatch C-API.
 *
 * Created: 17th June 2005
 * Updated: 17th September 2026
 *
 * Home:    https://github.com/synesissoftware/shwild.fnmatch
 *
 * Copyright (c) 2019-2026, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2005-2019, Matthew Wilson and Sean Kelly
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


/** \file shwild.fnmatch/shwild.fnmatch.hpp [C++] C++ root file for the
 *   **shwild.fnmatch** C-API
 */

#ifndef SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH
#define SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#ifndef SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH_MAJOR     1
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH_MINOR     1
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH_REVISION  0
# define SHWILD_FNMATCH_VER_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH_EDIT      4
#endif /* !SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifndef __cplusplus
# error This file can only be used in C++ compilation units
#endif /* !__cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#ifndef SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_H_SHWILD_FNMATCH
# include <shwild.fnmatch/shwild.fnmatch.h>
#endif /* !SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_H_SHWILD_FNMATCH */

#include <stdexcept>


/* /////////////////////////////////////////////////////////////////////////
 * compiler warnings
 */

#ifdef __BORLANDC__
# pragma warn -8026 /* Suppresses "Functions with exception specifications are not expanded inline" */
#endif /* compiler */


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(SHWILD_FNMATCH_NO_NAMESPACE)
namespace shwild
{
namespace fnmatch
{
#endif /* !SHWILD_FNMATCH_NO_NAMESPACE */


/* /////////////////////////////////////////////////////////////////////////
 * classes
 */

/// \brief Exception thrown by the Pattern constructor
/// \ingroup group__shwild_fnmatch_api__cpp_api
class PatternException
    : public std::runtime_error
{
/// \name Member Types
/// @{
public:
    typedef std::runtime_error  parent_class_type;
    typedef PatternException    class_type;
/// @}

/// \name Construction
/// @{
public:
    /// \brief Construct from the given message and error code
    PatternException(char const* message, int shwildErrorCode)
        : parent_class_type(message)
        , m_shwildErrorCode(shwildErrorCode)
    {}
/// @}

/// \name Accessors
/// @{
public:
    /// \brief The error code associated with the exception
    int errorCode() const throw()
    {
        return m_shwildErrorCode;
    }
/// @}

/// \name Members
/// @{
private:
    int m_shwildErrorCode;
/// @}
};

/// \brief Facade for the \ref group__shwild_fnmatch_api__c_api "shwild.fnmatch C API"
/// \ingroup group__shwild_fnmatch_api__cpp_api
class Pattern
{
public:
    /// \brief Parses and precompiles the given pattern, according to the behaviour specified by the given flags
    ///
    /// \note If the parsing fails, an instance of PatternException is thrown
    explicit Pattern(char const* pattern, unsigned flags = 0);
    /// \brief Releases any resources associated with the instance
    ~Pattern();

public:
    /// \brief Match the given string against the precompiled pattern maintained as member state
    bool match(char const* string) const;

private:
    static shwild_fnmatch_handle_t init_(char const* pattern, unsigned flags);

private:
    shwild_fnmatch_handle_t m_hCompiledPattern;

private:
    Pattern(Pattern const&);
    Pattern& operator =(Pattern const&);
};


/* /////////////////////////////////////////////////////////////////////////
 * implementation
 */

#ifndef SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION

inline
/* static */
shwild_fnmatch_handle_t
Pattern::init_(
    char const* pattern
,   unsigned    flags
)
{
    shwild_fnmatch_handle_t hCompiledPattern;
    int                     r = shwild_fnmatch_compile_pattern(pattern, flags, &hCompiledPattern);

    if (r < 0)
    {
        hCompiledPattern = NULL;

        throw PatternException("Failed to compile pattern", r);
    }

    return hCompiledPattern;
}

inline
Pattern::Pattern(char const* pattern, unsigned flags)
    : m_hCompiledPattern(init_(pattern, flags))
{}

inline
Pattern::~Pattern()
{
    shwild_fnmatch_destroy_pattern(m_hCompiledPattern);
}

inline
bool
Pattern::match(char const* string) const
{
    int r = shwild_fnmatch_match_pattern(m_hCompiledPattern, string);

    if (r < 0)
    {
        throw PatternException("Match failed", r);
    }

    return 0 == r;
}

#endif /* !SHWILD_FNMATCH_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(SHWILD_FNMATCH_NO_NAMESPACE)
} /* namespace fnmatch */
} /* namespace shwild */
#endif /* !SHWILD_FNMATCH_NO_NAMESPACE */


/* ////////////////////////////////////////////////////////////////////// */

#endif /* !SHWILD_FNMATCH_INCL_SHWILD_FNMATCH_HPP_SHWILD_FNMATCH */

/* ///////////////////////////// end of file //////////////////////////// */
