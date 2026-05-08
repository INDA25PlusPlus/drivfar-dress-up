#ifndef INTERACTION_H
#define INTERACTION_H

#include <CSFML/Window/Event.h>
#include "state.h"

/// Centralized function which handles ui state transitions based on key press
/// events.
void handleKeyPress(UiState *state, sfKeyEvent event);

#endif
