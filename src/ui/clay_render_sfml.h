#ifndef CLAY_RENDER_SFML_H
#define CLAY_RENDER_SFML_H

#include "clay.h"
#include "texture.h"
#include "ui/state.h"
#include <CSFML/Graphics.h>

typedef struct Clay_SfmlRenderData {
    sfRenderWindow *window;
    sfFont *const *fonts;
    const UiState *uiState;
} Clay_SfmlRenderData;

typedef struct Clay_SfmlImageData {
    const sfTexture *texture;
    Clay_Color color;
} Clay_SfmlImageData;

sfColor clayColorToSfColor(Clay_Color color);

Clay_Color sfColorToClayColor(sfColor color);

void *clayArenaAllocate(Clay_Arena *arena, size_t count, size_t elementSize);

Clay_Dimensions Clay_Sfml_MeasureText(Clay_StringSlice text,
                                      Clay_TextElementConfig *config,
                                      void *userData);

void Clay_Sfml_RenderClayCommands(Clay_SfmlRenderData *renderData,
                                  Clay_RenderCommandArray *rcommands);

#endif
