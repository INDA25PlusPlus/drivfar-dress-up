#include "garment_color_helper.h"

size_t garmentsToPaletteColors(const Garment garments[], size_t garmentCount,
                               PaletteColor outColors[], size_t maxColorCount)
{
    size_t colorCount = 0;

    for (size_t i = 0; i < garmentCount; i++) {
        if (colorCount >= maxColorCount) {
            break;
        }

        outColors[colorCount] = garments[i].color;
        colorCount++;
    }

    return colorCount;
}