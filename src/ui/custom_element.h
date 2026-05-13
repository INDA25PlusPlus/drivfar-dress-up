#ifndef CUSTOM_ELEMENT_H
#define CUSTOM_ELEMENT_H

/// Contains definitions

#include "ui/clay.h"
#include "ui/state.h"

typedef enum CustomElementType {
    /// Renders the
    CUSTOM_ELEMENT_DOLL,
} CustomElementType;

void renderCustomElementDoll(sfRenderWindow *window, const UiState *uiState,
                             Clay_BoundingBox bounds);

#endif
