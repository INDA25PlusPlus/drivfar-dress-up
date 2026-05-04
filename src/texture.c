

#include "texture.h"

/// Array of texture files (doll, background etc.), relative to the project root.
static const char *const texturePaths[] = { "assets/texture/doll.png" };

#define TEXTURE_COUNT (sizeof(texturePaths) / sizeof(*texturePaths))

sfTexture *textures[TEXTURE_COUNT] = { 0 };

bool loadTextures()
{
    for (size_t i = 0; i < TEXTURE_COUNT; i++) {
        const char *const path = texturePaths[i];

        sfTexture *texture = sfTexture_createFromFile(path, NULL);
        if (texture == NULL) {
            // Destroy existing textures.
            for (size_t j = 0; j < i; j++) {
                sfTexture_destroy(textures[j]);
            }

            return false;
        }

        textures[i] = texture;
    }

    return true;
}

void destroyTextures()
{
    for (size_t i = 0; i < TEXTURE_COUNT; i++) {
        sfTexture_destroy(textures[i]);
    }
}
