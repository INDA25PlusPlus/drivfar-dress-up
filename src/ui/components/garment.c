#include "garment.h"
#include "texture.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/key_hint.h"
#include "ui/state.h"
#include "ui/ui.h"
#include "ui/ui_color.h"

/// Draws a square representing a garment in the garment selector list widget.
static void GarmentIcon(const UiState *const state, const Garment garment)
{
    const GarmentAsset *garmentAsset = &garments[garment.id];
    Clay_BorderElementConfig border = { 0 };
    Clay_Color backgroundColor = { 0 };
    Clay_Color garmentColor = { 0 };

    if (uiStateSelectedGarmentType(state) == garmentAsset->type &&
        state->focusState == UI_FOCUS_REGION_GARMENTS) {
        // This garment's row is currently being edited.

        if (state->garmentsActive[garment.id]) {
            backgroundColor = UI_COLOR_ICON_ACCENT_FILL;
            garmentColor = sfColorToClayColor(colorToSfColor(garment.color));
        } else {
            backgroundColor = UI_COLOR_TRANSPARENT;
            garmentColor = UI_COLOR_ICON_ACCENT_UNSELECTED;
        }
    } else {
        if (state->garmentsActive[garment.id]) {
            backgroundColor = UI_COLOR_ICON_SELECTED_FILL;
            garmentColor = sfColorToClayColor(colorToSfColor(garment.color));
        } else {
            backgroundColor = UI_COLOR_TRANSPARENT;
            garmentColor = UI_COLOR_GARMENT_PREVIEW_FILL;
        }
    }
    if (state->selectedGarment == garment.id) {
        border = (Clay_BorderElementConfig){
            .color = UI_COLOR_ICON_ACCENT_SELECTED,
            .width = CLAY_BORDER_ALL(3),
        };
        backgroundColor = UI_COLOR_ICON_ACCENT_FILL;
    }

    Clay_SfmlImageData *coloredImage =
        clayArenaAllocate(&frameArena, 1, sizeof(Clay_SfmlImageData));
    *coloredImage =
        (Clay_SfmlImageData){ .texture = garmentAsset->coloredTexture,
                              .color = garmentColor };
    Clay_SfmlImageData *detailsImage =
        clayArenaAllocate(&frameArena, 1, sizeof(Clay_SfmlImageData));
    *detailsImage =
        (Clay_SfmlImageData){ .texture = garmentAsset->detailsTexture,
                              .color = UI_COLOR_WHITE };

    CLAY_AUTO_ID(
        { .layout.sizing = { CLAY_SIZING_FIXED(80), CLAY_SIZING_FIXED(80) },
          .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
          .backgroundColor = backgroundColor,
          .border = border,
          .cornerRadius = CLAY_CORNER_RADIUS(10) })
    {
        // Colored image
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_FIXED(60), CLAY_SIZING_FIXED(60) },
              .floating.attachTo = CLAY_ATTACH_TO_PARENT,
              .floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_CENTER,
              .floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_CENTER,
              .image.imageData = coloredImage })
        {
        }

        // Details image
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_FIXED(60), CLAY_SIZING_FIXED(60) },
              .floating.attachTo = CLAY_ATTACH_TO_PARENT,
              .floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_CENTER,
              .floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_CENTER,
              .image.imageData = detailsImage })
        {
        }
    }
}

/// Draws a single row of the garment selector list widget.
static void GarmentSelectorRow(const UiState *const state, GarmentType type)
{
    CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
                   .layout.childAlignment.y = CLAY_ALIGN_Y_CENTER,
                   .layout.childGap = 3,
                   //    .border = border,
                   .cornerRadius = CLAY_CORNER_RADIUS(10) })
    {
        // Icon
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_FIXED(70), CLAY_SIZING_FIXED(70) },
              .layout.childAlignment = { CLAY_ALIGN_X_CENTER,
                                         CLAY_ALIGN_Y_CENTER } })
        {
            TextureId texture;
            switch (type) {
            case GARMENT_TYPE_HAT: {
                texture = TEXTURE_ICON_GARMENT_HAT;
            } break;
            case GARMENT_TYPE_TOP: {
                texture = TEXTURE_ICON_GARMENT_TOP;
            } break;
            case GARMENT_TYPE_BOTTOM: {
                texture = TEXTURE_ICON_GARMENT_BOTTOM;
            } break;
            case GARMENT_TYPE_SHOES: {
                texture = TEXTURE_ICON_GARMENT_SHOES;
            } break;
            }

            Clay_Color color = uiStateSelectedGarmentType(state) == type &&
                                       state->focusState ==
                                           UI_FOCUS_REGION_GARMENTS ?
                                   UI_COLOR_ICON_ACCENT_SELECTED :
                                   UI_COLOR_ICON_UNSELECTED;

            Clay_SfmlImageData *image =
                clayArenaAllocate(&frameArena, 1, sizeof(Clay_SfmlImageData));
            *image = (Clay_SfmlImageData){ .texture = textures[texture],
                                           .color = color };
            CLAY_AUTO_ID({ .layout.sizing = { CLAY_SIZING_FIXED(30),
                                              CLAY_SIZING_FIXED(30) },
                           .image.imageData = image })
            {
            }
        }

        // Divider
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_FIXED(3), CLAY_SIZING_FIXED(60) },
              .backgroundColor = UI_COLOR_MAIN_BORDER,
              .cornerRadius = CLAY_CORNER_RADIUS(3) })
        {
        }

        // Garment icons
        CLAY_AUTO_ID({ .layout.childAlignment = { CLAY_ALIGN_X_LEFT,
                                                  CLAY_ALIGN_Y_CENTER },
                       .layout.childGap = 3 })
        {
            for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
                if (garments[id].type == type) {
                    GarmentIcon(state,
                                (Garment){ .id = id,
                                           .color = state->garmentsColor[id] });
                }
            }
        }
    }
}

void GarmentSelector(const UiState *const state)
{
    CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_TOP_TO_BOTTOM })
    {
        CLAY_AUTO_ID({ .layout.sizing.width = CLAY_SIZING_GROW(),
                       .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
                       .layout.padding = CLAY_PADDING_ALL(15),
                       .layout.childGap = 5,
                       .cornerRadius = { .topLeft = 10, .topRight = 10 },
                       .border.color = UI_COLOR_MAIN_BORDER,
                       .border.width = { .left = 5, .top = 5, .right = 5 } })
        {
            GarmentSelectorRow(state, GARMENT_TYPE_HAT);
            GarmentSelectorRow(state, GARMENT_TYPE_TOP);
            GarmentSelectorRow(state, GARMENT_TYPE_BOTTOM);
            GarmentSelectorRow(state, GARMENT_TYPE_SHOES);
        }

        // Key hints container
        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(31) },
              .layout.childGap = 5,
              .layout.padding = { .top = 3, .bottom = 3, .left = 5, .right = 5 },
              .cornerRadius = { .bottomLeft = 10, .bottomRight = 10 },
              .backgroundColor = UI_COLOR_MAIN_BORDER })
        {
            if (state->focusState == UI_FOCUS_REGION_GARMENTS) {
                KeyHint(textures[TEXTURE_ICON_STICK_VERTICAL],
                        CLAY_STRING("Select Type"), UI_COLOR_TEXT);
                KeyHint(textures[TEXTURE_ICON_STICK_HORIZONTAL],
                        CLAY_STRING("Select Garment"), UI_COLOR_TEXT);
                KeyHint(textures[TEXTURE_ICON_BUTTON_A],
                        CLAY_STRING("Add/Remove"), UI_COLOR_TEXT);
                KeyHint(textures[TEXTURE_ICON_BUTTON_B],
                        CLAY_STRING("Edit Color"),
                        state->garmentsActive[state->selectedGarment] ?
                            UI_COLOR_TEXT :
                            UI_COLOR_TRANSPARENT);
            }
        }
    }
}
