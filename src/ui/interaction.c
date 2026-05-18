#include <CSFML/Window/Event.h>
#include <err.h>

#include "color_grid.h"
#include "error_utilities.h"
#include "state.h"

#include "dpi_scale.h"

typedef enum XDirection {
    LEFT,
    RIGHT,
} XDirection;

typedef enum YDirection {
    ABOVE,
    BELOW,
} YDirection;

/// Get the index of a garment among the garments which share it's type. So if
/// there are only two top garments, regardless of their ID's the second top
/// garment will get an index of 1.
static size_t getGarmentIndexAmongType(GarmentId garment)
{
    size_t index = 0;
    for (GarmentId id = 0; id < garment; id++) {
        if (garments[id].type == garments[garment].type) {
            index += 1;
        }
    }
    return index;
}

/// Get the nth garment of the specified type. If the index doesn't exist then
/// the search results in the garment for the highest index. If no garments of
/// the specified type exist then the search has no result.
/// @param index The position among the garments of `type` to look for. If
///   there are only two top garments, regardless of their ID's the second top
///   garment will get an index of 1.
/// @return If a result was found, in which case the found garment is written to
///   `result`.
static bool getClosestGarmentAmongType(size_t index, GarmentType type,
                                       GarmentId *result)
{
    size_t nextIndex = 0;
    GarmentId currentId = 0;
    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        if (garments[id].type == type) {
            currentId = id;
            if (nextIndex == index) {
                *result = currentId;
                return true;
            }
            nextIndex += 1;
        }
    }

    if (nextIndex == 0) {
        return false;
    }

    *result = currentId;
    return true;
}

/// Searches all garment IDs for the first garment at or after the specified ID
/// which matches the specified type. A bool is returned specifying if a match
/// was found, in which case `result` is updated with the result.
static bool getNextGarment(GarmentId start, GarmentType type, GarmentId *result)
{
    for (GarmentId id = start; id < GARMENT_COUNT; id++) {
        if (garments[id].type == type) {
            *result = id;
            return true;
        }
    }

    return false;
}

/// Searches all garment IDs for the first garment at or before the specified ID
/// which matches the specified type. A bool is returned specifying if a match
/// was found, in which case `result` is updated with the result.
static bool getPreviousGarment(GarmentId start, GarmentType type,
                               GarmentId *result)
{
    for (GarmentId id = start + 1; id--;) {
        if (garments[id].type == type) {
            *result = id;
            return true;
        }
    }

    return false;
}

/// Selects the garment type for editing which is above or below the currently
/// selected type, clamping the selection to the top and bottom rows.
static void moveGarmentTypeSelectionY(UiState *state, YDirection direction)
{
    size_t currentIndex = getGarmentIndexAmongType(state->selectedGarment);

    // We have to loop the searching to support the case where there are no
    // garments of a specific type. This won't be a problem once we've added
    // garments of each time, but until then this is necessary.
    GarmentType searchType = garments[state->selectedGarment].type;
    while (true) {
        switch (searchType) {
        case GARMENT_TYPE_HAT: {
            if (direction == ABOVE) {
                // Reached end, no new selection is possible.
                return;
            }
            searchType = GARMENT_TYPE_TOP;
        } break;
        case GARMENT_TYPE_TOP: {
            searchType = direction == ABOVE ? GARMENT_TYPE_HAT :
                                              GARMENT_TYPE_BOTTOM;
        } break;
        case GARMENT_TYPE_BOTTOM: {
            searchType = direction == ABOVE ? GARMENT_TYPE_TOP :
                                              GARMENT_TYPE_SHOES;
        } break;
        case GARMENT_TYPE_SHOES: {
            if (direction == BELOW) {
                // Reached end, no new selection is possible.
                return;
            }
            searchType = GARMENT_TYPE_BOTTOM;
        } break;
        default:
            ASSERT_UNREACHABLE();
        }

        if (getClosestGarmentAmongType(currentIndex, searchType,
                                       &state->selectedGarment)) {
            return;
        }
    }
}

/// Selects the garment for editing which is to the left or right of the
/// currently selected garment, if that garment exists. Otherwise nothing is
/// done.
static void moveGarmentTypeSelectionX(UiState *state, XDirection direction)
{
    GarmentType type = uiStateSelectedGarmentType(state);
    if (direction == LEFT) {
        GarmentId result;
        if (state->selectedGarment != 0 &&
            getPreviousGarment(state->selectedGarment - 1, type, &result)) {
            state->selectedGarment = result;
        }
    } else {
        GarmentId result;
        if (getNextGarment(state->selectedGarment + 1, type, &result)) {
            state->selectedGarment = result;
        }
    }
}

static void gradeResult(UiState *state)
{
    size_t garmentCount = 0;
    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        if (state->garmentsActive[id]) {
            garmentCount += 1;
        }
    }

    PaletteColor *garmentColors = calloc(garmentCount, sizeof(PaletteColor));
    Garment *garmentStyles = calloc(garmentCount, sizeof(Garment));
    if (garmentColors == NULL || garmentStyles == NULL) {
        err(1, "Garment array allocation failed");
    }

    size_t garmentIndex = 0;
    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        if (state->garmentsActive[id]) {
            garmentColors[garmentIndex] = state->garmentsColor[id];
            garmentStyles[garmentIndex] =
                (Garment){ .id = id, .color = state->garmentsColor[id] };
            garmentIndex++;
        }
    }

    StyleResult styleResult = judgeStyle(garmentStyles, garmentCount);
    state->gradeResult =
        judgeGrade(judgeColorScheme(garmentColors, garmentCount), styleResult);

    free(garmentColors);
    free(garmentStyles);

    sfClock_restart(state->gradeScreenSwitchClock);
    state->focusState = UI_FOCUS_REGION_GRADE;
}

void handleKeyPress(UiState **state, sfKeyEvent event)
{
    switch (event.code) {
    case sfKeyO: {
        uiScale -= 0.1;
        printf("uiScale: %f\n", uiScale);
    } break;
    case sfKeyI: {
        uiScale += 0.1;
        printf("uiScale: %f\n", uiScale);
    } break;
    default:
        break;
    }
    switch ((*state)->focusState) {
    case UI_FOCUS_REGION_GARMENTS: {
        switch (event.code) {
        case sfKeyA:
        case sfKeyNum1: {
            (*state)->garmentsActive[(*state)->selectedGarment] =
                !(*state)->garmentsActive[(*state)->selectedGarment];
        } break;
        case sfKeyB:
        case sfKeyNum2: {
            // We only allow editing the color of garments placed on the doll.
            if ((*state)->garmentsActive[(*state)->selectedGarment]) {
                (*state)->focusState = UI_FOCUS_REGION_COLORS;
            }
        } break;
        case sfKeyUp:
        case sfKeyT: {
            moveGarmentTypeSelectionY((*state), ABOVE);
        } break;
        case sfKeyDown:
        case sfKeyG: {
            moveGarmentTypeSelectionY((*state), BELOW);
        } break;
        case sfKeyLeft:
        case sfKeyF: {
            moveGarmentTypeSelectionX((*state), LEFT);
        } break;
        case sfKeyRight:
        case sfKeyH: {
            moveGarmentTypeSelectionX((*state), RIGHT);
        } break;
        case sfKeyEnter: {
            (*state)->focusState = UI_FOCUS_REGION_GRADE_CONFIRMATION;
        } break;
        default:
            break;
        }
    } break;
    case UI_FOCUS_REGION_COLORS: {
        switch (event.code) {
        case sfKeyB:
        case sfKeyNum2: {
            (*state)->focusState = UI_FOCUS_REGION_GARMENTS;
        } break;
        case sfKeyUp:
        case sfKeyT: {
            PaletteColor *color =
                &(*state)->garmentsColor[(*state)->selectedGarment];
            *color = colorGridGetAdjacentColor(*color, GRID_DIRECTION_ABOVE);
        } break;
        case sfKeyDown:
        case sfKeyG: {
            PaletteColor *color =
                &(*state)->garmentsColor[(*state)->selectedGarment];
            *color = colorGridGetAdjacentColor(*color, GRID_DIRECTION_BELOW);
        } break;
        case sfKeyLeft:
        case sfKeyF: {
            PaletteColor *color =
                &(*state)->garmentsColor[(*state)->selectedGarment];
            *color = colorGridGetAdjacentColor(*color, GRID_DIRECTION_LEFT);
        } break;
        case sfKeyRight:
        case sfKeyH: {
            PaletteColor *color =
                &(*state)->garmentsColor[(*state)->selectedGarment];
            *color = colorGridGetAdjacentColor(*color, GRID_DIRECTION_RIGHT);
        } break;
        case sfKeyEnter: {
            (*state)->focusState = UI_FOCUS_REGION_GRADE_CONFIRMATION;
        } break;
        default:
            break;
        }
    } break;
    case UI_FOCUS_REGION_GRADE: {
        switch (event.code) {
        case sfKeyEnter: {
            uiStateDestroy(*state);
            *state = uiStateCreate();
            (*state)->focusState = UI_FOCUS_REGION_GARMENTS;
        } break;
        default:
            break;
        }
    } break;
    default:
        break;
    }
}

void handleKeyRelease(UiState **state, sfKeyEvent event)
{
    switch ((*state)->focusState) {
    case UI_FOCUS_REGION_GRADE_CONFIRMATION: {
        switch (event.code) {
        case sfKeyA:
        case sfKeyNum1: {
            (*state)->focusState = UI_FOCUS_REGION_GARMENTS;
        } break;
        case sfKeyB:
        case sfKeyNum2: {
            gradeResult((*state));
        } break;
        default:
            break;
        }
    } break;
    default:
        break;
    }
}
