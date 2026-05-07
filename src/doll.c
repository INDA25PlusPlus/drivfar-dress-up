

#include <stdlib.h>
#include <assert.h>
#include "doll.h"
#include "texture.h"
#include "garment_list.h"

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
}
