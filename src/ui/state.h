#ifndef STATE_H
#define STATE_H

#include "garment.h"

/// Represents a part of the UI which can be "focused", i.e. that is at a time
/// solely consuming input.
typedef enum UiFocusRegion {
    UI_FOCUS_REGION_GARMENTS = 0,
    UI_FOCUS_REGION_COLORS,
} UiFocusRegion;

typedef struct UiState {
    /// For every defined garment, whether or not it's currently active.
    bool garmentsActive[GARMENT_COUNT];
    /// For every defined garment, the color selected for that garment.
    PaletteColor garmentsColor[GARMENT_COUNT];
    /// Which part of the UI is currently in focus.
    UiFocusRegion focusState;
    /// The garment which currently has focus in the garment list widget. This
    /// also defines which garment type row is selected.
    GarmentId selectedGarment;
} UiState;

static inline GarmentType uiStateSelectedGarmentType(const UiState *const state)
{
    return garments[state->selectedGarment].type;
}

#endif
