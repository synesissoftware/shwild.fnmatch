/* /////////////////////////////////////////////////////////////////////////
 * File:    test.unit.shwild_fnmatch_match.c
 *
 * Purpose: Unit-tests for the shwild.fnmatch C API.
 *
 * Created: 8th May 2006
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <shwild.fnmatch/shwild.fnmatch.h>

#include <xtests/terse-api.h>

#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * helper functions
 */

static
int
test_compiled(
    char const* pattern
,   char const* string
,   unsigned    flags
)
{
    shwild_fnmatch_handle_t hCompiledPattern;
    int                     res = shwild_fnmatch_compile_pattern(pattern, flags, &hCompiledPattern);

    if (res >= 0)
    {
        res = shwild_fnmatch_match_pattern(hCompiledPattern, string);

        shwild_fnmatch_destroy_pattern(hCompiledPattern);
    }

    return res;
}

/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void test_literals(void);
static void test_ignorecase(void);
static void test_escape(void);
static void test_period(void);
static void test_pathname(void);
static void test_pathname_period(void);
static void test_compile_pattern(void);
#if defined(_WIN32) || \
    defined(WIN32)
static void test_win32_pathname(void);
#endif


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity;

    XTESTS_COMMANDLINE_PARSE_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.shwild_fnmatch_match", verbosity))
    {
        XTESTS_RUN_CASE(test_literals);
        XTESTS_RUN_CASE(test_ignorecase);
        XTESTS_RUN_CASE(test_escape);
        XTESTS_RUN_CASE(test_period);
        XTESTS_RUN_CASE(test_pathname);
        XTESTS_RUN_CASE(test_pathname_period);
        XTESTS_RUN_CASE(test_compile_pattern);
#if defined(_WIN32) || \
    defined(WIN32)
        XTESTS_RUN_CASE(test_win32_pathname);
#endif

        XTESTS_PRINT_RESULTS();
        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function implementations
 */

static void test_literals(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("a", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("a", "b", 0 | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("*", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("*a", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("a*", "a", 0 | defaultFlags));
}

static void test_ignorecase(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("a", "A", SHWILD_FNMATCH_F_IGNORECASE | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("A", "a", SHWILD_FNMATCH_F_IGNORECASE | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("a", "A", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("A", "a", 0 | defaultFlags));
}

static void test_escape(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?", "?", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?", "\\", 0 | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("\\?", "?", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "\\", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "\\?", 0 | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "a", SHWILD_FNMATCH_F_NOESCAPE | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "?", SHWILD_FNMATCH_F_NOESCAPE | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("\\?", "\\", SHWILD_FNMATCH_F_NOESCAPE | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("\\?", "\\?", SHWILD_FNMATCH_F_NOESCAPE | defaultFlags));
}

static void test_period(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match(".a", ".a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?a", "aa", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?a", ".a", 0 | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match(".a", ".a", SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("?a", "aa", SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("?a", ".a", SHWILD_FNMATCH_F_PERIOD | defaultFlags));
}

static void test_pathname(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("*", "/usr/include/stlsoft", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "/usr/include/stlsoft", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/usr/*/*", "/usr/include/stlsoft", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/usr/*", "/usr/include/stlsoft", 0 | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/usr/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("/usr/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
}

static void test_pathname_period(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/usr/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("/usr/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("*", "/.usr/.include/.stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("/*/*/*", "/.usr/.include/.stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/.*/.*/.*", "/.usr/.include/.stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD | defaultFlags));
}

static void test_compile_pattern(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("a", "a", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("1", "b", 0 | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("/*/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("/usr/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
}

#if defined(_WIN32) || \
    defined(WIN32)
static void test_win32_pathname(void)
{
    unsigned const defaultFlags = SHWILD_FNMATCH_F_NOIGNORECASE;

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "/STLSoft/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("/*/*/*", "\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_NOSLASHSWAP | SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, shwild_fnmatch_match("/*/*/*", "H:\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, shwild_fnmatch_match("/*/*/*", "H:\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_NOIMPLICITROOT | SHWILD_FNMATCH_F_PATHNAME | defaultFlags));

    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("/*/*/*", "\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("/*/*/*", "H:\\STLSoft\\include\\stlsoft", SHWILD_FNMATCH_F_PATHNAME | defaultFlags));
}
#endif /* windows */


/* ///////////////////////////// end of file //////////////////////////// */
