

#include <stdlib.h>
#include <assert.h>
#include "doll.h"
#include "texture.h"
#include "garment_list.h"

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

void renderDoll(sfRenderWindow *window, Doll *doll)
{
    sfVector2u windowSize = sfRenderWindow_getSize(window);

    // doll position with offset
    sfVector2f dollOffset = calcOffset(doll->sprite);
    sfVector2f dollPos = {
        windowSize.x * 2.5f + dollOffset.x, windowSize.y * 0.9f
    }; // no offset for y since we want feet on ground for standing doll ??

    sfFloatRect bounds = sfSprite_getLocalBounds(doll->sprite);

    float currentHeight = bounds.size.y;
    float desiredHeight = windowSize.y * 0.8;

    float s = desiredHeight / currentHeight;

    sfTransform transform = sfTransform_Identity;

    sfTransform_scale(&transform, (sfVector2f){ s, s });
    sfTransform_translate(&transform, dollPos);

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
        sfSprite *coloredSprite = sfSprite_create(asset->coloredTexture);
        sfSprite_setColor(coloredSprite, colorToSfColor(g->color));

        sfFloatRect b = sfSprite_getLocalBounds(coloredSprite);

        sfVector2f colorCenteredPos = { asset->position.x + b.size.x / 2.f,
                                        asset->position.y + b.size.y / 2.f };

        sfSprite_setPosition(coloredSprite, colorCenteredPos);
        sfSprite_setScale(coloredSprite, asset->scale);

        sfRenderWindow_drawSprite(window, coloredSprite, &states);
        sfSprite_destroy(coloredSprite);

        // details layer
        sfSprite *detailsSprite = sfSprite_create(asset->detailsTexture);

        b = sfSprite_getLocalBounds(coloredSprite);

        sfVector2f detailCenteredPos = { asset->position.x + b.size.x / 2.f,
                                         asset->position.y + b.size.y / 2.f };

        sfSprite_setPosition(detailsSprite, detailCenteredPos);
        sfSprite_setScale(detailsSprite, asset->scale);

        sfRenderWindow_drawSprite(window, detailsSprite, &states);
        sfSprite_destroy(detailsSprite);
    }
}

sfVector2f calcOffset(sfSprite *sprite)
{
    sfFloatRect bounds = sfSprite_getLocalBounds(sprite);

    return (sfVector2f){ bounds.size.x / 2, bounds.size.y / 2 };
}