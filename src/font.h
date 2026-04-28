#ifndef FONT_H
#define FONT_H

#include <CSFML/Graphics.h>

/// Hard coded list of fonts used in the project.
typedef enum {
    // The names should be prefixed by `FONT_`.
    FONT_ARIAL,
} FontId;

/// The list of loaded garment assets. Is indexed using `GarmentId`.
///
/// Must not be accessed before `loadFonts` has been called.
extern sfFont *fonts[];

/// Load font assets. Returns false if error is encountered.
bool loadFonts();

/// Destroys font assets, making `fonts` invalid to access.
void destroyFonts();

#endif
