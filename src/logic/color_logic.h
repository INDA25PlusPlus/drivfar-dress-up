#ifndef COLOR_LOGIC_H
#define COLOR_LOGIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../garment.h"

typedef enum {
    COLOR_SCHEME_NONE,
    COLOR_SCHEME_COMPLEMENTARY,
    COLOR_SCHEME_ANALOGOUS,
    COLOR_SCHEME_TRIADIC,
    COLOR_SCHEME_SPLIT_COMPLEMENTARY,
} ColorScheme;

ColorScheme judgeColorScheme(const PaletteColor colors[], size_t colorCount);

#endif