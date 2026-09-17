/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/cpp/example.cpp.pattern/example.cpp.pattern.cpp
 *
 * Purpose: C++ example of shwild::fnmatch::Pattern.
 *
 * Created: 17th September 2026
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <shwild.fnmatch/shwild.fnmatch.hpp>

#include <cstdio>
#include <cstdlib>


int main(int /*argc*/, char* /*argv*/[])
{
    shwild::fnmatch::Pattern pattern("/usr/*/*", SHWILD_FNMATCH_F_PATHNAME);

    if (!pattern.match("/usr/include/stlsoft"))
    {
        std::fprintf(stderr, "expected compiled PATHNAME match\n");

        return EXIT_FAILURE;
    }

    if (pattern.match("/usr/include"))
    {
        std::fprintf(stderr, "expected compiled PATHNAME non-match\n");

        return EXIT_FAILURE;
    }

    std::printf("shwild.fnmatch C++ Pattern example: ok\n");

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
