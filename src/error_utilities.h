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

#define ASSERT_UNKNOWN_GARMENT_STYLE() \
    assert(!"Unrecognized garment style in styleToString")
#define ASSERT_UNKNOWN_GARMENT_ID_STYLE() \
    assert(!"Unrecognized garment ID in styleForGarment")
    
#define ASSERT_UNKNOWN_COLOR_SCHEME_GRADING() \
    assert(!"Unrecognized color scheme in grading logic")
#define ASSERT_UNKNOWN_GARMENT_STYLE_GRADING() \
    assert(!"Unrecognized garment style in grading logic")

#endif
