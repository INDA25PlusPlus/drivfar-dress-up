#include <stdio.h>
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>

#include "font.h"
#include "garment.h"
#include "ui/ui.h"
#include "texture.h"
#include "doll.h"

typedef enum { PAGE_DRESS_UP, PAGE_GRADING } Page;

int main(void)
{
    if (!loadGarments()) {
        return 1;
    }
    if (!loadFonts()) {
        return 1;
    }
    if (!loadTextures()) {
        return 1;
    }

    sfVideoMode mode = { { 1200, 1080 }, 32 };
    sfContextSettings contextSettings = (sfContextSettings){
        .minorVersion = 1, .majorVersion = 1, .antiAliasingLevel = 4
    };
    sfRenderWindow *window =
        sfRenderWindow_create(mode, "Dress Up Skeleton", sfResize | sfClose,
                              sfWindowed, &contextSettings);
    if (!window)
        return 1;

    g_doll = dollCreate((sfVector2f){ 400, 20 });
    // Test adding garments
    // garmentListPush(g_doll->garments, (Garment){
    //     .id = GARMENT_TEST_A,
    //     .color = COLOR_RED
    // });

    // garmentListPush(g_doll->garments, (Garment){
    //     .id = GARMENT_PANTS_A,
    //     .color = COLOR_CERISE
    // });
    start_ui(window);

    sfRenderWindow_destroy(window);
    destroyFonts();

    return 0;
}