#ifndef ERROR_UTILITIES_H
#define ERROR_UTILITIES_H

/// Contains utilities for handling errors or unreachable code paths.

#include <stdio.h>
#include <stdlib.h>

#define ASSERT_UNREACHABLE() \
    (fprintf(stderr, "Reached unreachable code\n"), exit(101))
#define ASSERT_UNREACHABLE_MSG(msgString) \
    (fprintf(stderr, "Assertion failed: %s\n", msgString), exit(101))

#define ASSERT_UNKNOWN_STYLE() \
    assert(!"Unrecognized garment ID in styleForGarment")

#endif
