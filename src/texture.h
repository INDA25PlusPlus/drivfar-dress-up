#ifndef TEXTURE_H
#define TEXTURE_H

#include "CSFML/Graphics.h"

extern sfTexture *textures[];

/// Hard coded list of fonts used in the project.
typedef enum {
    // The names should be prefixed by `TEXTURE_`.
    TEXTURE_DOLL,
} TextureId;

/// Load texture assets. Returns false if error is encountered.
bool loadTextures();

/// Destroys texture assets, making `textures` invalid to access.
void destroyTextures();

#endif