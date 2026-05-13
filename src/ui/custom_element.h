#ifndef CUSTOM_ELEMENT_H
#define CUSTOM_ELEMENT_H

/// Contains definitions

#include "ui/clay.h"
#include "ui/state.h"

/// List of all custom Clay elements. Currently only contains the doll
/// component. Each type should have a corresponding rendering function, which
/// is called in 'clay_render_sfml.c'.
typedef enum CustomElementType {
    /// Renders the
    CUSTOM_ELEMENT_DOLL,
} CustomElementType;

void renderCustomElementDoll(sfRenderWindow *window, const UiState *uiState,
                             Clay_BoundingBox bounds);

#endif
