#include <CSFML/Graphics.h>

#include "font.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/key_hint.h"
#include "ui/custom_element.h"
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
            CustomElementType *type =
                clayArenaAllocate(&frameArena, 1, sizeof(CustomElementType));
            *type = CUSTOM_ELEMENT_DOLL;

            CLAY_AUTO_ID({
                .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
                .custom.customData = type,
            });
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
