#ifndef APP_H
#define APP_H

#include "clay.h"
#include <CSFML/Graphics.h>
#include "doll.h"

extern Doll *g_doll;

/// Arena used for custom data allocation. Is reset every frame.
extern Clay_Arena frameArena;

void start_ui(sfRenderWindow *window);

#endif
