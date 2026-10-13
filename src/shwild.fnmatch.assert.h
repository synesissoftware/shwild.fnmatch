/* /////////////////////////////////////////////////////////////////////////
 * File:    src/shwild.fnmatch.assert.h
 *
 * Purpose: Internal assertions for the shwild.fnmatch implementation.
 *
 * Created: 25th February 2007
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#ifndef SHWILD_FNMATCH_INCL_SRC_H_SHWILD_FNMATCH_ASSERT
#define SHWILD_FNMATCH_INCL_SRC_H_SHWILD_FNMATCH_ASSERT

#include <assert.h>

#define SHWILD_FNMATCH_ASSERT(expr)                         assert((expr))

#define SHWILD_FNMATCH_MESSAGE_ASSERT(msg, expr)            assert((expr))

#endif /* !SHWILD_FNMATCH_INCL_SRC_H_SHWILD_FNMATCH_ASSERT */

/* ///////////////////////////// end of file //////////////////////////// */
