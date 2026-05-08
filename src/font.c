#include <CSFML/Graphics.h>

#include "font.h"

/// Array of font files, relative to the project root.
static const char *const fontPaths[] = {
    "assets/font/arial.ttf", "assets/font/karrik/Karrik-Regular.ttf"
};

#define FONT_COUNT (sizeof(fontPaths) / sizeof(*fontPaths))

sfFont *fonts[FONT_COUNT] = { 0 };

bool loadFonts()
{
    for (size_t i = 0; i < FONT_COUNT; i++) {
        const char *const path = fontPaths[i];

        sfFont *font = sfFont_createFromFile(path);
        if (font == NULL) {
            // Destroy existing fonts.
            for (size_t j = 0; i < j; j++) {
                sfFont_destroy(fonts[j]);
            }

            return false;
        }

        fonts[i] = font;
    }

    return true;
}

void destroyFonts()
{
    for (size_t i = 0; i < FONT_COUNT; i++) {
        sfFont_destroy(fonts[i]);
    }
}
