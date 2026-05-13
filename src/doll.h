#ifndef DOLL_H
#define DOLL_H

#include <CSFML/Graphics.h>
#include "texture.h"
#include "garment_list.h"

#define MAX_GARMENTS 8

typedef struct {
    sfSprite *sprite;
    sfTexture *texture;

    GarmentList *garments;

} Doll;

// renders the doll and its garmets
void renderDoll(sfRenderWindow *window, Doll *doll, sfVector2f position,
                float height);
void dollDestroy(Doll *doll);

Doll *dollCreate();

#endif