#ifndef KEY_HINT_H
#define KEY_HINT_H

#include <CSFML/Graphics.h>

#include "ui/clay.h"

/// Draws a key hint element, which consists of an icon and a text hint.
void KeyHint(const sfTexture *icon, Clay_String text, Clay_Color color);

#endif
