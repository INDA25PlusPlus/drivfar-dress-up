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

#include "color_ui.h"
#include "clay_render_sfml.h"
#include "font.h"
#include "ui.h"

Doll *g_doll = NULL;

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
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(
        totalMemorySize, malloc(totalMemorySize));

    sfVector2u windowSize = sfRenderWindow_getSize(window);
    Clay_Initialize(arena, (Clay_Dimensions){ windowSize.x, windowSize.y },
                    (Clay_ErrorHandler){ handle_clay_errors });

    Clay_SfmlRenderData renderData = {
        .window = window,
        .fonts = fonts,
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
            }
        }

        sfVector2u windowSize = sfRenderWindow_getSize(window);
        Clay_SetLayoutDimensions(
            (Clay_Dimensions){ windowSize.x, windowSize.y });

        sfVector2i mousePosition = sfMouse_getPositionRenderWindow(window);
        Clay_SetPointerState((Clay_Vector2){ mousePosition.x, mousePosition.y },
                             sfMouse_isButtonPressed(sfMouseLeft));

        Clay_BeginLayout();

        CLAY(CLAY_ID("Outer"), { .layout = { .sizing = { CLAY_SIZING_GROW(0),
                                                         CLAY_SIZING_GROW(0) },
                                             .padding = CLAY_PADDING_ALL(16),
                                             .childGap = 16 },
                                 .backgroundColor = { 250, 250, 255, 255 } })
        {
            CLAY(CLAY_ID("SideBar"),
                 { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                               .sizing = { .width = CLAY_SIZING_FIXED(300),
                                           .height = CLAY_SIZING_GROW(0) },
                               .padding = CLAY_PADDING_ALL(16),
                               .childGap = 8,
                                 },
                   .backgroundColor = UI_COLOR_BASE,
                 })

            {
                CLAY_AUTO_ID(
                    { .layout = { .sizing = { .width = CLAY_SIZING_GROW(1),
                                              .height = CLAY_SIZING_FIXED(5) } },
                      .backgroundColor = UI_COLOR_BLACK })
                {
                }

                CLAY_AUTO_ID(
                    { .layout = { .sizing = { .width = CLAY_SIZING_GROW(1),
                                              .height = CLAY_SIZING_FIT() } } })
                {
                    CLAY_TEXT(CLAY_STRING("Clay - Test :3"),
                              { .fontId = FONT_ARIAL,
                                .fontSize = 24,
                                .textColor = { 255, 255, 255, 255 } });
                    CLAY_TEXT(CLAY_STRING("|"),
                              { .fontId = FONT_ARIAL,
                                .fontSize = 24,
                                .textColor = { 255, 255, 255, 255 } });
                    CLAY_TEXT(CLAY_STRING("a"),
                              { .fontId = FONT_ARIAL,
                                .fontSize = 24,
                                .textColor = { 255, 255, 255, 255 } });
                    CLAY_TEXT(CLAY_STRING("A"),
                              { .fontId = FONT_ARIAL,
                                .fontSize = 24,
                                .textColor = { 255, 255, 255, 255 } });
                }

                CLAY_AUTO_ID(
                    { .layout = { .sizing = { .width = CLAY_SIZING_GROW(1),
                                              .height = CLAY_SIZING_FIXED(5) } },
                      .backgroundColor = UI_COLOR_BLACK })
                {
                }
                CLAY_AUTO_ID(
                    { .layout = { .sizing = { .width = CLAY_SIZING_GROW(1),
                                              .height = CLAY_SIZING_FIXED(5) } },
                      .backgroundColor = UI_COLOR_BLACK })
                {
                }

                CLAY(CLAY_ID("Button"),
                     { .layout = { .sizing = { .width = CLAY_SIZING_FIT(),
                                               .height = CLAY_SIZING_FIT() },
                                   .padding = CLAY_PADDING_ALL(16) },
                       .backgroundColor = Clay_Hovered() ?
                                              UI_COLOR_BASE_HOVERED :
                                              UI_COLOR_BASE,
                       .cornerRadius = CLAY_CORNER_RADIUS(16),
                       .border = { .color = UI_COLOR_BORDER,
                                   .width = CLAY_BORDER_ALL(4) } })
                {
                    Clay_OnHover(HandleButtonInteraction, NULL);

                    CLAY_TEXT(CLAY_STRING("Click Me!"),
                              { .fontId = FONT_ARIAL,
                                .fontSize = 24,
                                .letterSpacing = 2,
                                .textColor = { 255, 255, 255, 255 } });
                }

                CLAY_AUTO_ID(
                    { .layout = { .sizing = { .width = CLAY_SIZING_GROW(0, 100),
                                              .height =
                                                  CLAY_SIZING_FIXED(50) } },
                      .cornerRadius = CLAY_CORNER_RADIUS(16),
                      .border = { .color = UI_COLOR_BORDER,
                                  .width = { .left = 10,
                                             .right = 5,
                                             .top = 3,
                                             .bottom = 10 } } })
                {
                }
            }

            CLAY(CLAY_ID("MainContent"),
                 { .layout = { .sizing = { .width = CLAY_SIZING_GROW(0),
                                           .height = CLAY_SIZING_GROW(0) },
                               .padding = CLAY_PADDING_ALL(16) },
                   .backgroundColor = UI_COLOR_BASE,
                   .cornerRadius = { 16, 16, 16, 16 },
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
                      .textColor = { 200, 200, 200, 255 } });
            }
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(delta_secs);

        sfRenderWindow_clear(window, sfBlack);

        Clay_Sfml_RenderClayCommands(&renderData, &renderCommands);

        renderDoll(window, g_doll);

        sfRenderWindow_display(window);
    }

    sfClock_destroy(clock);
}
