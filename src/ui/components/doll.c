#include <CSFML/Graphics.h>

#include "font.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/key_hint.h"
#include "ui/ui.h"
#include "ui/ui_color.h"

/// Draws the doll view frame.
void DollView()
{
    CLAY_AUTO_ID({ .layout.sizing = { .width = CLAY_SIZING_GROW(0),
                                      .height = CLAY_SIZING_GROW(0) },
                   .layout.layoutDirection = CLAY_TOP_TO_BOTTOM })
    {
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
              .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
              .layout.padding = CLAY_PADDING_ALL(15),
              .layout.childGap = 5,
              .cornerRadius = { .topLeft = 20, .topRight = 20 },
              .border.color = UI_COLOR_MAIN_BORDER,
              .border.width = { .left = 5, .top = 5, .right = 5 },
              .clip = { .vertical = true,
                        .childOffset = Clay_GetScrollOffset() } })
        {
            CLAY_TEXT(
                CLAY_STRING(
                    "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum."),
                { .fontId = FONT_ARIAL,
                  .fontSize = 24,
                  .letterSpacing = 2,
                  .lineHeight = 50,
                  .textColor = UI_COLOR_TEXT });
        }

        // Key hints container
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(31) },
              .layout.childGap = 5,
              .layout.padding = { .top = 3, .bottom = 3, .left = 5, .right = 5 },
              .cornerRadius = { .bottomLeft = 10, .bottomRight = 10 },
              .backgroundColor = UI_COLOR_MAIN_BORDER })
        {
            KeyHint(textures[TEXTURE_ICON_BUTTON_START],
                    CLAY_STRING("Submit for Grading"), UI_COLOR_TEXT);
        }
    }
}
