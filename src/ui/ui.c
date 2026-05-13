// clay.h seems to generate some warnings, this suppresses them.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#define CLAY_IMPLEMENTATION
#include "clay.h"

// Restore previous configuration.
#pragma GCC diagnostic pop

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <CSFML/Graphics.h>

#include "clay_render_sfml.h"
#include "components/color.h"
#include "components/doll.h"
#include "components/garment.h"
#include "components/grade.h"
#include "dpi_scale.h"
#include "font.h"
#include "interaction.h"
#include "state.h"
#include "ui_color.h"
#include "ui.h"

Doll *g_doll = NULL;

Clay_Arena frameArena = {};

static void handle_clay_errors(Clay_ErrorData error_data)
{
    // See the Clay_ErrorData struct for more information
    printf("Clay error: %s\n", error_data.errorText.chars);
}

void HandleButtonInteraction(Clay_ElementId elementId,
                             Clay_PointerData pointerData, void *userData)
{
    // Pointer state allows you to detect mouse down / hold / release
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        // Do some click handling
        printf("Clicked!\n");
    }
}

void start_ui(sfRenderWindow *window)
{
    loadUiScale();

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(
        totalMemorySize, malloc(totalMemorySize));

    uint64_t frameArenaSize = 1024 * 10;
    frameArena = Clay_CreateArenaWithCapacityAndMemory(frameArenaSize,
                                                       malloc(frameArenaSize));

    sfVector2u windowSize = sfRenderWindow_getSize(window);
    Clay_Initialize(arena,
                    (Clay_Dimensions){ windowSize.x / uiScale,
                                       windowSize.y / uiScale },
                    (Clay_ErrorHandler){ handle_clay_errors });

    UiState *state = uiStateCreate();

    Clay_SfmlRenderData renderData = {
        .window = window,
        .fonts = fonts,
        .uiState = state,
    };
    Clay_SetMeasureTextFunction(Clay_Sfml_MeasureText, &renderData);

    sfClock *clock = sfClock_create();

    while (sfRenderWindow_isOpen(window)) {
        sfTime elapsed = sfClock_restart(clock);
        float delta_secs = sfTime_asSeconds(elapsed);
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            } else if (event.type == sfEvtResized) {
                sfRenderWindow_setSize(window, event.size.size);
                sfView *view = sfView_createFromRect((sfFloatRect){
                    { 0.0, 0.0 },
                    { (float)event.size.size.x, (float)event.size.size.y } });
                if (!view) {
                    err(1, "Failed allocating view\n");
                }
                sfRenderWindow_setView(window, view);
                sfView_destroy(view);
            } else if (event.type == sfEvtMouseWheelScrolled) {
                switch (event.mouseWheelScroll.wheel) {
                case sfMouseVerticalWheel: {
                    Clay_UpdateScrollContainers(
                        false,
                        (Clay_Vector2){ 0, event.mouseWheelScroll.delta },
                        delta_secs);
                } break;
                case sfMouseHorizontalWheel: {
                    Clay_UpdateScrollContainers(
                        false,
                        (Clay_Vector2){ event.mouseWheelScroll.delta, 0 },
                        delta_secs);
                } break;
                }
            } else if (event.type == sfEvtKeyPressed) {
                handleKeyPress(&state, event.key);
            } else if (event.type == sfEvtKeyReleased) {
                handleKeyRelease(&state, event.key);
            }
        }

        sfVector2u windowSize = sfRenderWindow_getSize(window);
        Clay_SetLayoutDimensions((Clay_Dimensions){ windowSize.x / uiScale,
                                                    windowSize.y / uiScale });

        sfVector2i mousePosition = sfMouse_getPositionRenderWindow(window);
        Clay_SetPointerState((Clay_Vector2){ mousePosition.x / uiScale,
                                             mousePosition.y / uiScale },
                             sfMouse_isButtonPressed(sfMouseLeft));

        Clay_BeginLayout();

        CLAY_AUTO_ID(
            { .layout.sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
              .layout.layoutDirection = CLAY_TOP_TO_BOTTOM,
              .layout.childAlignment.x = CLAY_ALIGN_X_RIGHT,
              .layout.padding = CLAY_PADDING_ALL(25),
              .layout.childGap = 25,
              .backgroundColor = UI_COLOR_MAIN_FILL })
        {
            CLAY_TEXT(CLAY_STRING("DRIFVARDRESSING"),
                      { .fontId = FONT_KARRIK,
                        .fontSize = 52,
                        .lineHeight = (uint16_t)(52 * 1.2),
                        .textColor = UI_COLOR_TEXT });

            CLAY_AUTO_ID(
                { .layout.sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                  .layout.childGap = 25,
                  .layout.childAlignment.x = CLAY_ALIGN_X_CENTER,
                  .backgroundColor = UI_COLOR_MAIN_FILL })
            {
                if (state->focusState == UI_FOCUS_REGION_GRADE) {
                    GradeView(state);
                } else {
                    // Side bar
                    CLAY_AUTO_ID(
                         { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                       .sizing = { .width = CLAY_SIZING_FIT(),
                                                   .height = CLAY_SIZING_GROW() },
                                       .childGap = 10,
                                         },
                            .clip = { .vertical = true,
                                 .childOffset = Clay_GetScrollOffset() }
                         })
                    {
                        GarmentSelector(state);
                        ColorSelector(state);
                    }

                    DollView();
                }
            }
        }

        if (state->focusState == UI_FOCUS_REGION_GRADE_CONFIRMATION) {
            GradeConfirmationModal();
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(delta_secs);

        sfRenderWindow_clear(window, sfBlack);

        Clay_Sfml_RenderClayCommands(&renderData, &renderCommands);

        sfRenderWindow_display(window);

        frameArena.nextAllocation = 0;

        // Sleep if frame rate above 60.
        float elapsedSecondsEnd =
            sfTime_asSeconds(sfClock_getElapsedTime(clock));
        if (elapsedSecondsEnd < 1.0 / 60.0) {
            sfSleep(sfSeconds(1.0 / 60.0 - elapsedSecondsEnd));
        }
    }

    uiStateDestroy(state);

    sfClock_destroy(clock);
}
