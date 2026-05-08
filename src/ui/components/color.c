#include <CSFML/Graphics.h>

#include "ui/color_grid.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/key_hint.h"
#include "ui/state.h"
#include "ui/ui_color.h"

/// Width in pixels of the individual color squares' borders in the grid.
#define COLOR_SQUARE_BORDER_WIDTH 3

static void ColorSquare(const UiState *state, PaletteColor color)
{
    Clay_BorderElementConfig border = { 0 };
    if (state->focusState == UI_FOCUS_REGION_COLORS &&
        state->garmentsColor[state->selectedGarment] == color) {
        border = (Clay_BorderElementConfig){
            .width = CLAY_BORDER_ALL(COLOR_SQUARE_BORDER_WIDTH),
            .color = UI_COLOR_ICON_ACCENT_SELECTED,
        };
    }
    const float coloredSquareCornerRadius = 5;

    const float highlightCornerRadius =
        coloredSquareCornerRadius + COLOR_SQUARE_BORDER_WIDTH;
    // Colored square + neutral border
    CLAY_AUTO_ID(
        { .layout.sizing.width =
              CLAY_SIZING_FIXED(40 + COLOR_SQUARE_BORDER_WIDTH),
          .aspectRatio = { 1 },
          .cornerRadius = CLAY_CORNER_RADIUS(coloredSquareCornerRadius),
          .border.width = CLAY_BORDER_ALL(COLOR_SQUARE_BORDER_WIDTH),
          .border.color = UI_COLOR_BLACK,
          .backgroundColor = sfColorToClayColor(colorToSfColor(color)) })
    {
        // Highlight border
        CLAY_AUTO_ID(
            { .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
              .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
              .floating.attachTo = CLAY_ATTACH_TO_PARENT,
              .floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_CENTER,
              .floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_CENTER,
              .floating.expand = { COLOR_SQUARE_BORDER_WIDTH,
                                   COLOR_SQUARE_BORDER_WIDTH },
              .aspectRatio = { 1 },
              .cornerRadius = CLAY_CORNER_RADIUS(highlightCornerRadius),
              .border = border });
    }
}

/// Draws the grid allowing the user to select the colors used for garments.
void ColorSelector(const UiState *state)
{
    CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_TOP_TO_BOTTOM })
    {
        // Colors container
        CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
                       .layout.padding = CLAY_PADDING_ALL(15),
                       .layout.childGap = 5,
                       .cornerRadius = { .topLeft = 10, .topRight = 10 },
                       .border.color = UI_COLOR_MAIN_BORDER,
                       .border.width = { .left = 5, .top = 5, .right = 5 } })
        {
            for (size_t y = 0; y < COLOR_GRID_HEIGHT; y++) {
                CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
                               .layout.childGap = 5 })
                {
                    for (size_t x = 0; x < COLOR_GRID_WIDTH; x++) {
                        ColorSquare(state, colorGridGetColorAtPosition(
                                               (sfVector2u){ x, y }));
                    }
                }
            }
        }

        // Key hints container
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(31) },
              .layout.childGap = 5,
              .layout.padding = { .top = 3, .bottom = 3, .left = 5, .right = 5 },
              .cornerRadius = { .bottomLeft = 10, .bottomRight = 10 },
              .backgroundColor = UI_COLOR_MAIN_BORDER })
        {
            if (state->focusState == UI_FOCUS_REGION_COLORS) {
                KeyHint(textures[TEXTURE_ICON_STICK], CLAY_STRING("Select"),
                        UI_COLOR_TEXT);
                KeyHint(textures[TEXTURE_ICON_BUTTON_B], CLAY_STRING("Confirm"),
                        UI_COLOR_TEXT);
            }
        }
    }
}
