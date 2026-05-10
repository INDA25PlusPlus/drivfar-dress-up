#ifndef STATE_H
#define STATE_H

#include "garment.h"
#include "logic/grading_logic.h"

/// Represents a part of the UI which can be "focused", i.e. that is at a time
/// solely consuming input.
typedef enum UiFocusRegion {
    UI_FOCUS_REGION_GARMENTS = 0,
    UI_FOCUS_REGION_COLORS,
    UI_FOCUS_REGION_GRADE_CONFIRMATION,
    UI_FOCUS_REGION_GRADE,
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
    /// Clock which counts the time since the grade screen was switched to. Used
    /// for animations. Must be reset once the grade screen is switched to.
    sfClock *gradeScreenSwitchClock;
    /// Populated with the result after the user has submitted for grading.
    GradeResult gradeResult;

} UiState;

UiState *uiStateCreate();

void uiStateDestroy(UiState *state);

static inline GarmentType uiStateSelectedGarmentType(const UiState *const state)
{
    return garments[state->selectedGarment].type;
}

#endif
