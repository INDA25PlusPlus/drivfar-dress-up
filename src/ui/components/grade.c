#include "CSFML/Graphics.h"

#include "error_utilities.h"
#include "font.h"
#include "garment.h"
#include "ui/clay.h"
#include "ui/clay_render_sfml.h"
#include "ui/components/grade.h"
#include "ui/ui_color.h"
#include "ui/ui.h"
#include "texture.h"

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

static Clay_String defaultGradeDescription(Grade grade)
{
    switch (grade) {
    case GRADE_A:
        return CLAY_STRING("Perfect!");
    case GRADE_B:
        return CLAY_STRING("Superb");
    case GRADE_C:
        return CLAY_STRING("OK");
    case GRADE_D:
        return CLAY_STRING("Passable");
    case GRADE_E:
        return CLAY_STRING("Try again");
    case GRADE_F:
        return CLAY_STRING("What are you doing?");
    }

    ASSERT_UNREACHABLE();
    return CLAY_STRING("");
}

// TODO: Improve these descriptions.
static Clay_String gradeBasisDescription(GradeResult gradeResult)
{
    switch (gradeResult.basis) {
    case GRADE_BASIS_COLOR_COMPLEMENTARY:
        return CLAY_STRING("That color really complements you!");
    case GRADE_BASIS_COLOR_ANALOGOUS:
        return CLAY_STRING("Those colors flow nicely together.");
    case GRADE_BASIS_COLOR_TRIADIC:
        return CLAY_STRING("Bold, but balanced.");
    case GRADE_BASIS_COLOR_SPLIT_COMPLEMENTARY:
        return CLAY_STRING("Perfect contrast.");
    case GRADE_BASIS_STYLE_COHERENT_GASQUE:
        return CLAY_STRING("I wanna be where you're going!");
    case GRADE_BASIS_STYLE_COHERENT_SITTNING:
        return CLAY_STRING(
            "Elegant. Don't forget the correct order for cheering your drinks!");
    case GRADE_BASIS_STYLE_COHERENT_PUB:
        return CLAY_STRING(
            "That outfit knows where the evening is going."); //idk
    case GRADE_BASIS_STYLE_COHERENT_OVERALL:
        return CLAY_STRING(
            "Är det redan fredag?"); // Visst är det fredag man "ska" bära ovve?
    case GRADE_BASIS_STYLE_CLASH:
        return CLAY_STRING("You're gonna wear THOSE together?");
    case GRADE_BASIS_NONE:
        return defaultGradeDescription(gradeResult.grade);
    }

    ASSERT_UNREACHABLE();
    return CLAY_STRING("");
}

void GradeView(UiState *state)
{
    CLAY_AUTO_ID(
        { .layout.layoutDirection = CLAY_TOP_TO_BOTTOM, .layout.childGap = 5 })
    {
        CLAY_TEXT(CLAY_STRING("Result"),
                  { .fontId = FONT_KARRIK,
                    .fontSize = 40,
                    .textColor = UI_COLOR_TEXT,
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER });
        CLAY_AUTO_ID({
            .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
            .layout.childGap = 25,
        })
        {
            // Doll display
            CLAY_AUTO_ID({
                .layout.sizing = { CLAY_SIZING_FIXED(200),
                                   CLAY_SIZING_GROW(500) },
                .cornerRadius = CLAY_CORNER_RADIUS(10),
                .backgroundColor = UI_COLOR_MAIN_FILL,
                .border.width = CLAY_BORDER_OUTSIDE(5),
                .border.color = UI_COLOR_MAIN_BORDER,
            });

            // Results
            CLAY_AUTO_ID({ .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
                           .layout.sizing.height = CLAY_SIZING_GROW(),
                           .layout.padding = CLAY_PADDING_ALL(25),
                           .cornerRadius = CLAY_CORNER_RADIUS(10),
                           .backgroundColor = UI_COLOR_MAIN_FILL,
                           .border.width = CLAY_BORDER_OUTSIDE(5),
                           .border.color = UI_COLOR_MAIN_BORDER })
            {
                // Maybe show color (and style) score here?

                Clay_String colorSchemeString;

                switch (state->gradeResult.colorScheme) {
                case COLOR_SCHEME_NONE: {
                    colorSchemeString = CLAY_STRING("N/A");
                } break;
                case COLOR_SCHEME_COMPLEMENTARY: {
                    colorSchemeString = CLAY_STRING("COMPLEMENTARY");
                } break;
                case COLOR_SCHEME_ANALOGOUS: {
                    colorSchemeString = CLAY_STRING("ANALOGOUS");
                } break;
                case COLOR_SCHEME_TRIADIC: {
                    colorSchemeString = CLAY_STRING("TRIADIC");
                } break;
                case COLOR_SCHEME_SPLIT_COMPLEMENTARY: {
                    colorSchemeString = CLAY_STRING("SPLIT_COMPLEMENTARY");
                } break;
                default:
                    ASSERT_UNREACHABLE();
                }

                CLAY_AUTO_ID({
                    .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .layout.childAlignment.y = CLAY_ALIGN_Y_BOTTOM,
                    .layout.childGap = 10,
                })
                {
                    // TODO: Animations would be cool here...

                    CLAY_TEXT(CLAY_STRING("Color Scheme:"),
                              {
                                  .fontId = FONT_KARRIK,
                                  .fontSize = 24,
                                  .textColor = UI_COLOR_TEXT_NEUTRAL,
                              });

                    CLAY_TEXT(colorSchemeString,
                              {
                                  .fontId = FONT_KARRIK,
                                  .fontSize = 30,
                                  .textColor = UI_COLOR_TEXT_NEUTRAL,
                              });
                }

                CLAY_AUTO_ID({ .layout.sizing.height = CLAY_SIZING_GROW() });

                Clay_String gradeString;
                Clay_String gradeDescriptionString =
                    gradeBasisDescription(state->gradeResult);
                switch (state->gradeResult.grade) {
                case GRADE_A: {
                    gradeString = CLAY_STRING("A");
                } break;
                case GRADE_B: {
                    gradeString = CLAY_STRING("B");
                } break;
                case GRADE_C: {
                    gradeString = CLAY_STRING("C");
                } break;
                case GRADE_D: {
                    gradeString = CLAY_STRING("D");
                } break;
                case GRADE_E: {
                    gradeString = CLAY_STRING("E");
                } break;
                case GRADE_F: {
                    gradeString = CLAY_STRING("F");
                } break;
                default:
                    ASSERT_UNREACHABLE();
                }

                CLAY_AUTO_ID({
                    .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .layout.childAlignment.y = CLAY_ALIGN_Y_BOTTOM,
                    .layout.childGap = 10,
                })
                {
                    // TODO: Animations would be cool here...

                    CLAY_TEXT(CLAY_STRING("Grade:"),
                              {
                                  .fontId = FONT_KARRIK,
                                  .fontSize = 24,
                                  .textColor = UI_COLOR_ICON_ACCENT_SELECTED,
                              });

                    CLAY_TEXT(gradeString,
                              {
                                  .fontId = FONT_KARRIK,
                                  .fontSize = 70,
                                  .textColor = UI_COLOR_TEXT_NEUTRAL,
                              });
                }

                CLAY_TEXT(gradeDescriptionString,
                          {
                              .fontId = FONT_KARRIK,
                              .fontSize = 30,
                              .textColor = UI_COLOR_TEXT_NEUTRAL,
                          });
            }
        }

        CLAY_AUTO_ID({
            .layout.layoutDirection = CLAY_LEFT_TO_RIGHT,
            .layout.sizing.width = CLAY_SIZING_GROW(),
            .layout.childAlignment.x = CLAY_ALIGN_X_CENTER,
            .layout.childAlignment.y = CLAY_ALIGN_Y_CENTER,
        })
        {
            CLAY_TEXT(CLAY_STRING("Press"), { .fontId = FONT_KARRIK,
                                              .fontSize = 24,
                                              .textColor = UI_COLOR_TEXT });

            {
                sfTexture *icon = textures[TEXTURE_ICON_BUTTON_START];
                Clay_SfmlImageData *image = clayArenaAllocate(
                    &frameArena, 1, sizeof(Clay_SfmlImageData));
                *image = (Clay_SfmlImageData){ .texture = icon,
                                               .color = UI_COLOR_TEXT };
                const float aspectRatio = (float)sfTexture_getSize(icon).x /
                                          (float)sfTexture_getSize(icon).y;
                static const float height = 36;

                CLAY_AUTO_ID({ .layout.padding.top = 6 })
                {
                    CLAY_AUTO_ID(
                        { .layout.sizing.width =
                              CLAY_SIZING_FIXED(height * aspectRatio),
                          .layout.sizing.height = CLAY_SIZING_FIXED(height),
                          .image.imageData = image });
                }
            }

            CLAY_TEXT(CLAY_STRING("to try again"),
                      { .fontId = FONT_KARRIK,
                        .fontSize = 24,
                        .textColor = UI_COLOR_TEXT });
        }
    }
}
