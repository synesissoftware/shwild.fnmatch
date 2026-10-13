/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.cpp
 *
 * Purpose: Prints shwild.fnmatch composite version and efferent deps,
 *          using Diagnosticism for rich version strings.
 *
 * Created: 17th September 2026
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <shwild.fnmatch/shwild.fnmatch.h>

#include <cstring/cstring.h>
#include <diagnosticism/version_string.hpp>
#include <shwild/shwild.h>
#include <stlsoft/stlsoft.h>
#include <xtests/xtests.h>

#include <iomanip>
#include <iostream>
#include <string>

#include <stdlib.h>


#define PROGRAM_NAME                                        "test.scratch.versions"


template<
    typename T_stream
,   typename T_integer
>
void
version(
    T_stream&   stm
,   char const* prefix
,   char const* libname
,   char const* macroname
,   T_integer   libver
,   int         verMajor
,   int         verMinor
,   int         verPatch
,   int         verAlphaBeta
)
{
    std::string const rich = diagnosticism::calc_version_string(
        verMajor
    ,   verMinor
    ,   verPatch
    ,   verAlphaBeta
    );

    stm
        << prefix
        << libname
        << ": v"
        << rich
        << " ("
        << macroname
        << " = 0x"
        << std::hex << std::setfill('0') << std::setw(8)
        << static_cast<unsigned>(libver)
        << std::dec
        << ")"
        << std::endl
        ;
}


int main(int /* argc */, char* /* argv */[])
{
    {
        unsigned const libver = SHWILD_FNMATCH_VER;

        version(
            std::cout
        ,   ""
        ,   "shwild.fnmatch"
        ,   "SHWILD_FNMATCH_VER"
        ,   libver
        ,   SHWILD_FNMATCH_VER_MAJOR
        ,   SHWILD_FNMATCH_VER_MINOR
        ,   SHWILD_FNMATCH_VER_PATCH
        ,   SHWILD_FNMATCH_VER_ALPHABETA
        );
    }

    std::cout << "\n" << "efferent dependencies:" << std::endl;

    {
        unsigned const libver = CSTRING_VER;

        version(
            std::cout
        ,   "\t"
        ,   "cstring"
        ,   "CSTRING_VER"
        ,   libver
        ,   CSTRING_VER_MAJOR
        ,   CSTRING_VER_MINOR
        ,   CSTRING_VER_PATCH
        ,   CSTRING_VER_ALPHABETA
        );
    }

    {
        unsigned const libver = DIAGNOSTICISM_VER;

        version(
            std::cout
        ,   "\t"
        ,   "Diagnosticism"
        ,   "DIAGNOSTICISM_VER"
        ,   libver
        ,   DIAGNOSTICISM_VER_MAJOR
        ,   DIAGNOSTICISM_VER_MINOR
        ,   DIAGNOSTICISM_VER_PATCH
        ,   DIAGNOSTICISM_VER_ALPHABETA
        );
    }

    {
        unsigned const libver = SHWILD_VER;

        version(
            std::cout
        ,   "\t"
        ,   "shwild"
        ,   "SHWILD_VER"
        ,   libver
        ,   SHWILD_VER_MAJOR
        ,   SHWILD_VER_MINOR
        ,   SHWILD_VER_PATCH
        ,   SHWILD_VER_ALPHABETA
        );
    }

    {
        unsigned const libver = _STLSOFT_VER;

        version(
            std::cout
        ,   "\t"
        ,   "STLSoft"
        ,   "_STLSOFT_VER"
        ,   libver
        ,   _STLSOFT_VER_MAJOR
        ,   _STLSOFT_VER_MINOR
        ,   _STLSOFT_VER_PATCH
        ,   _STLSOFT_VER_ALPHABETA
        );
    }

    {
        unsigned const libver = _XTESTS_VER;

        version(
            std::cout
        ,   "\t"
        ,   "xTests"
        ,   "_XTESTS_VER"
        ,   libver
        ,   _XTESTS_VER_MAJOR
        ,   _XTESTS_VER_MINOR
        ,   _XTESTS_VER_PATCH
        ,   _XTESTS_VER_ALPHABETA
        );
    }

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
