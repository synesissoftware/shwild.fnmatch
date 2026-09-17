/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/c/example.c.match/example.c.match.c
 *
 * Purpose: C example of shwild_fnmatch_match().
 *
 * Created: 17th September 2026
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <shwild.fnmatch/shwild.fnmatch.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    int r;

    ((void)argc);
    ((void)argv);

    r = shwild_fnmatch_match("/usr/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME);
    if (SHWILD_FNMATCH_RC_SUCCESS != r)
    {
        fprintf(stderr, "expected PATHNAME match\n");

        return EXIT_FAILURE;
    }

    r = shwild_fnmatch_match("/usr/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME);
    if (SHWILD_FNMATCH_RC_NOMATCH != r)
    {
        fprintf(stderr, "expected PATHNAME non-match\n");

        return EXIT_FAILURE;
    }

    printf("shwild.fnmatch C match example: ok\n");

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
