#include "CSFML/Graphics.h"

#include "garment.h"
#include "texture.h"
#include "font.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/grade.h"
#include "ui/ui_color.h"
#include "ui/ui.h"

static void Button(sfTexture *icon, Clay_String label,
                   Clay_Color backgroundColor, Clay_Color textColor)
{
    CLAY_AUTO_ID(
        { .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
          .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
          .layout.childGap = 2,
          .layout.padding =
              (Clay_Padding){ .left = 5, .right = 12, .top = 5, .bottom = 5 },
          .cornerRadius = CLAY_CORNER_RADIUS(100),
          .backgroundColor = backgroundColor })
    {
        Clay_SfmlImageData *image =
            clayArenaAllocate(&frameArena, 1, sizeof(Clay_SfmlImageData));
        *image = (Clay_SfmlImageData){ .texture = icon, .color = textColor };

        const float aspectRatio =
            (float)sfTexture_getSize(icon).x / (float)sfTexture_getSize(icon).y;
        static const float height = 30.0;

        CLAY_AUTO_ID(
            { .layout.sizing.width = CLAY_SIZING_FIXED(height * aspectRatio),
              .aspectRatio = { aspectRatio },
              .image.imageData = image });

        // Add manual padding since sfml can't center text vertically properly
        CLAY_AUTO_ID({ .layout.padding.bottom = 5 })
        {
            CLAY_TEXT(label, { .fontId = FONT_KARRIK,
                               .fontSize = 20,
                               .textColor = textColor });
        }
    }
}

void GradeConfirmationModal()
{
    // Background fade
    CLAY_AUTO_ID(
        { .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
          .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
          .floating.attachTo = CLAY_ATTACH_TO_ROOT,
          .backgroundColor = UI_COLOR_DIM_BACKGROUND })
    {
        // Modal frame
        CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
                       .layout.childAlignment.x = CLAY_ALIGN_X_CENTER,
                       .layout.padding = CLAY_PADDING_ALL(15),
                       .layout.childGap = 15,
                       .cornerRadius = CLAY_CORNER_RADIUS(20),
                       .backgroundColor = UI_COLOR_MAIN_FILL,
                       .border.width = CLAY_BORDER_OUTSIDE(5),
                       .border.color = UI_COLOR_MAIN_BORDER })
        {
            CLAY_TEXT(CLAY_STRING("Do you want to submit for grading?"),
                      { .fontId = FONT_KARRIK,
                        .fontSize = 24,
                        .textColor = UI_COLOR_TEXT,
                        .textAlignment = CLAY_TEXT_ALIGN_CENTER });

            CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
                           .layout.childGap = 10 })
            {
                // Cancel button
                bool cancelPressed = sfKeyboard_isKeyPressed(sfKeyA) ||
                                     sfKeyboard_isKeyPressed(sfKeyNum1);
                Clay_Color cancelColor = UI_COLOR_BUTTON_FILL;
                Clay_Color cancelTextColor = UI_COLOR_MAIN_FILL;
                if (cancelPressed) {
                    cancelColor.a = 150;
                }
                Button(textures[TEXTURE_ICON_BUTTON_A], CLAY_STRING("Cancel"),
                       cancelColor, cancelTextColor);

                // Submit button
                bool submitPressed = sfKeyboard_isKeyPressed(sfKeyB) ||
                                     sfKeyboard_isKeyPressed(sfKeyNum2);
                Clay_Color submitColor = UI_COLOR_ICON_ACCENT_SELECTED;
                Clay_Color submitTextColor = UI_COLOR_MAIN_FILL;
                if (submitPressed) {
                    submitColor.a = 150;
                }
                Button(textures[TEXTURE_ICON_BUTTON_B], CLAY_STRING("Submit"),
                       submitColor, submitTextColor);
            }
        }
    }
}
