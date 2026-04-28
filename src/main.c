#include <stdio.h>
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>
#include "font.h"
#include "garment.h"
#include "ui/ui.h"

typedef enum { PAGE_DRESS_UP, PAGE_GRADING } Page;

int main(void)
{
    if (!loadGarments()) {
        return 1;
    }
    if (!loadFonts()) {
        return 1;
    }
    sfVideoMode mode = { { 800, 600 }, 32 };
    sfRenderWindow *window = sfRenderWindow_create(
        mode, "Dress Up Skeleton", sfResize | sfClose, sfWindowed, NULL);
    if (!window)
        return 1;

    start_ui(window);

    sfRenderWindow_destroy(window);
    destroyFonts();

    return 0;
}