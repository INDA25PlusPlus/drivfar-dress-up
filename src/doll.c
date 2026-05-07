

#include <stdlib.h>
#include <assert.h>
#include "doll.h"
#include "texture.h"
#include "garment_list.h"

#include <CSFML/Graphics.h>

Doll *dollCreate(sfVector2f position)
{
    Doll *doll = calloc(1, sizeof(Doll));
    assert(doll);

    doll->position = position;

    // create sprite from texture
    doll->texture = textures[TEXTURE_DOLL];
    doll->sprite = sfSprite_create(doll->texture);
    assert(doll->sprite);

    // scale image of doll
    sfSprite_setScale(doll->sprite, (sfVector2f){ 0.8f, 0.8f });

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
    // draw base doll
    sfSprite_setPosition(doll->sprite, doll->position);
    sfRenderWindow_drawSprite(window, doll->sprite, NULL);

    // Draw garments on top
    // TODO: Draw in correct order (first bottoms, then tops etc??)
    for (size_t i = 0; i < doll->garments->len; i++) {
        Garment *g = &doll->garments->items[i];
        GarmentAsset *asset = &garments[g->id];

        // position relative to doll
        sfVector2f pos = { doll->position.x + asset->position.x,
                           doll->position.y + asset->position.y };

        // colored layer
        sfSprite *coloredSprite = sfSprite_create(asset->coloredTexture);
        sfSprite_setPosition(coloredSprite, pos);
        sfSprite_setScale(coloredSprite, asset->scale);
        sfSprite_setColor(coloredSprite, colorToSfColor(g->color));

        sfRenderWindow_drawSprite(window, coloredSprite, NULL);
        sfSprite_destroy(coloredSprite);

        // details layer
        sfSprite *detailsSprite = sfSprite_create(asset->detailsTexture);
        sfSprite_setPosition(detailsSprite, pos);
        sfSprite_setScale(detailsSprite, asset->scale);

        sfRenderWindow_drawSprite(window, detailsSprite, NULL);
        sfSprite_destroy(detailsSprite);
    }
}
