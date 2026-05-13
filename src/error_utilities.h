#ifndef ERROR_UTILITIES_H
#define ERROR_UTILITIES_H

/// Contains utilities for handling errors or unreachable code paths.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT_UNREACHABLE() \
    (fprintf(stderr, "Reached unreachable code\n"), exit(101))
#define ASSERT_UNREACHABLE_MSG(msgString) \
    (fprintf(stderr, "Assertion failed: %s\n", msgString), exit(101))

#define ASSERT_UNKNOWN_GARMENT_STYLE() assert(!"Unrecognized garment style")
#define ASSERT_UNKNOWN_GARMENT_ID() assert(!"Unrecognized garment ID")

#endif
