#include <CSFML/Graphics.h>

#include "font.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/ui.h"
#include "ui/ui_color.h"

#include <stdio.h>

void KeyHint(const sfTexture *icon, Clay_String text, Clay_Color color)
{
    CLAY_AUTO_ID({ .layout.childAlignment.y = CLAY_ALIGN_Y_CENTER })
    {
        Clay_SfmlImageData *image =
            clayArenaAllocate(&frameArena, 1, sizeof(Clay_SfmlImageData));
        *image = (Clay_SfmlImageData){ .texture = icon, .color = color };

        float aspectRatio =
            (float)sfTexture_getSize(icon).x / (float)sfTexture_getSize(icon).y;

        CLAY_AUTO_ID(
            { .layout.sizing.width = CLAY_SIZING_FIXED(25.0 * aspectRatio),
              .aspectRatio = { aspectRatio },
              .image.imageData = image });
        CLAY_TEXT(text, { .fontSize = 16,
                          .fontId = FONT_KARRIK,
                          .textColor = color,
                          .wrapMode = CLAY_TEXT_WRAP_NONE });
    }
}
