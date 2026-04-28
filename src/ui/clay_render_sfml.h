#ifndef CLAY_RENDER_SFML_H
#define CLAY_RENDER_SFML_H

#include "clay.h"
#include <CSFML/Graphics.h>

typedef struct {
    sfRenderWindow *window;
    sfFont *const *fonts;
} Clay_SfmlRenderData;

Clay_Dimensions Clay_Sfml_MeasureText(Clay_StringSlice text,
                                      Clay_TextElementConfig *config,
                                      void *userData);

void Clay_Sfml_RenderClayCommands(Clay_SfmlRenderData *rendererData,
                                  Clay_RenderCommandArray *rcommands);

#endif
