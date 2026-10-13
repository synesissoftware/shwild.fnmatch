/* /////////////////////////////////////////////////////////////////////////
 * File:    test.unit.shwild.fnmatch.pattern.cpp
 *
 * Purpose: Unit-tests for shwild::fnmatch::Pattern.
 *
 * Created: 15th May 2006
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <shwild.fnmatch/shwild.fnmatch.hpp>

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
,   unsigned    flags = 0
)
{
    try
    {
        shwild::fnmatch::Pattern compiled(pattern, flags);

        return compiled.match(string) ? SHWILD_FNMATCH_RC_SUCCESS : SHWILD_FNMATCH_RC_NOMATCH;
    }
    catch (shwild::fnmatch::PatternException& x)
    {
        return x.errorCode();
    }
}


static void test_literals(void);
static void test_ignorecase(void);
static void test_escape(void);
static void test_period(void);
static void test_pathname(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity;

    XTESTS_COMMANDLINE_PARSE_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.shwild.fnmatch.pattern", verbosity))
    {
        XTESTS_RUN_CASE(test_literals);
        XTESTS_RUN_CASE(test_ignorecase);
        XTESTS_RUN_CASE(test_escape);
        XTESTS_RUN_CASE(test_period);
        XTESTS_RUN_CASE(test_pathname);

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
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("a", "a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("1", "b"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("?", "a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("*", "a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("*a", "a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("a*", "a"));
}

static void test_ignorecase(void)
{
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("a", "A", SHWILD_FNMATCH_F_IGNORECASE));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("A", "a", SHWILD_FNMATCH_F_IGNORECASE));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("a", "A", SHWILD_FNMATCH_F_NOIGNORECASE));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("A", "a", SHWILD_FNMATCH_F_NOIGNORECASE));
}

static void test_escape(void)
{
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("\\?", "a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("\\?", "?"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("\\?", "\\?", SHWILD_FNMATCH_F_NOESCAPE));
}

static void test_period(void)
{
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("?a", ".a"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("?a", ".a", SHWILD_FNMATCH_F_PERIOD));
}

static void test_pathname(void)
{
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("*", "/usr/include/stlsoft"));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("/*/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_NOMATCH, test_compiled("/usr/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME));
    TEST_INT_EQ(SHWILD_FNMATCH_RC_SUCCESS, test_compiled("/.*/.*/.*", "/.usr/.include/.stlsoft", SHWILD_FNMATCH_F_PATHNAME | SHWILD_FNMATCH_F_PERIOD));
}


/* ///////////////////////////// end of file //////////////////////////// */
