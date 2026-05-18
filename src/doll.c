

#include <stdlib.h>
#include <assert.h>
#include "doll.h"
#include "texture.h"
#include "garment_list.h"
#include "ui/dpi_scale.h"

#include <CSFML/Graphics.h>

Doll *dollCreate()
{
    Doll *doll = calloc(1, sizeof(Doll));
    assert(doll);

    // create sprite from texture
    doll->texture = textures[TEXTURE_DOLL];
    doll->sprite = sfSprite_create(doll->texture);
    assert(doll->sprite);

    // create garment list
    doll->garments = garmentListCreate();
    assert(doll->garments);

    return doll;
}

void dollDestroy(Doll *doll)
{
    if (!doll)
        return;

    if (doll->sprite)
        sfSprite_destroy(doll->sprite);

    garmentListDestroy(doll->garments);

    free(doll);
}

void renderDoll(sfRenderWindow *window, Doll *doll, sfVector2f position,
                float height)
{
    sfFloatRect bounds = sfSprite_getLocalBounds(doll->sprite);

    float currentHeight = bounds.size.y;
    float scale = height / currentHeight;

    sfVector2f centeredPosition = { position.x - bounds.size.x * scale / 2,
                                    position.y - bounds.size.y * scale / 2 };

    sfTransform transform = sfTransform_Identity;
    sfTransform_scale(&transform, (sfVector2f){ uiScale, uiScale });
    sfTransform_translate(&transform, centeredPosition);
    sfTransform_scale(&transform, (sfVector2f){ scale, scale });

    // define states so that doll and garment use same transform

    sfRenderStates states = { .blendMode = sfBlendAlpha,
                              .transform = transform,
                              .texture = NULL,
                              .shader = NULL };

    // draw base doll
    sfRenderWindow_drawSprite(window, doll->sprite, &states);

    // Draw garments on doll
    for (size_t i = 0; i < doll->garments->len; i++) {
        Garment *g = &doll->garments->items[i];
        GarmentAsset *asset = &garments[g->id];

        // colored layer
        if (asset->coloredTexture != NULL) {
            sfSprite *coloredSprite = sfSprite_create(asset->coloredTexture);
            sfSprite_setColor(coloredSprite, colorToSfColor(g->color));

            sfSprite_setPosition(coloredSprite, asset->position);
            sfSprite_setScale(coloredSprite, asset->scale);

            sfRenderWindow_drawSprite(window, coloredSprite, &states);
            sfSprite_destroy(coloredSprite);
        }

        // details layer
        if (asset->detailsTexture != NULL) {
            sfSprite *detailsSprite = sfSprite_create(asset->detailsTexture);

            sfSprite_setPosition(detailsSprite, asset->position);
            sfSprite_setScale(detailsSprite, asset->scale);

            sfRenderWindow_drawSprite(window, detailsSprite, &states);
            sfSprite_destroy(detailsSprite);
        }
    }
}
