#ifndef GARMENT_COLOR_HELPER_H
#define GARMENT_COLOR_HELPER_H

#include <stddef.h>
#include "../garment.h"

// Extracts the selected PaletteColor from each configured garment.
//
// Parameters:
// - garments: array of configured garments
// - garmentCount: number of garments in the garments array
// - outColors: array where the extracted colors will be written
// - maxColorCount: size of outColors
//
// Returns:
// - number of colors written to outColors
//
// Example:
// Garment outfit[] = {
//     { .id = GARMENT_TEST_A, .color = COLOR_CERISE_STRONG },
//     { .id = GARMENT_PANTS_A, .color = COLOR_GREEN_STRONG },
// };
//
// PaletteColor colors[2];
// size_t colorCount = garmentsToPaletteColors(outfit, 2, colors, 2);
size_t garmentsToPaletteColors(const Garment garments[], size_t garmentCount,
                               PaletteColor outColors[], size_t maxColorCount);

#endif