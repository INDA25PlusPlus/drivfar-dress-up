#ifndef TEXTURE_H
#define TEXTURE_H

#include "CSFML/Graphics.h"

extern sfTexture *textures[];

/// Hard coded list of fonts used in the project.
typedef enum {
    // The names should be prefixed by `TEXTURE_`.
    TEXTURE_DOLL,
    TEXTURE_ICON_GARMENT_HAT,
    TEXTURE_ICON_GARMENT_TOP,
    TEXTURE_ICON_GARMENT_BOTTOM,
    TEXTURE_ICON_GARMENT_SHOES,
    TEXTURE_ICON_STICK,
    TEXTURE_ICON_STICK_HORIZONTAL,
    TEXTURE_ICON_STICK_VERTICAL,
    TEXTURE_ICON_BUTTON_A,
    TEXTURE_ICON_BUTTON_B,
    TEXTURE_ICON_BUTTON_START,
    TEXTURE_ICON_BUTTON_SELECT,
} TextureId;

/// Load texture assets. Returns false if error is encountered.
bool loadTextures();

/// Destroys texture assets, making `textures` invalid to access.
void destroyTextures();

#endif
