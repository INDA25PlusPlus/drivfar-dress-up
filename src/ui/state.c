#include "ui/state.h"
#include "CSFML/Graphics.h"

UiState *uiStateCreate()
{
    UiState *state = calloc(1, sizeof(UiState));
    if (state == NULL) {
        return NULL;
    }
    state->gradeScreenSwitchClock = sfClock_create();
    if (state->gradeScreenSwitchClock == NULL) {
        free(state);
        return NULL;
    }
    return state;
}

void uiStateDestroy(UiState *state)
{
    sfClock_destroy(state->gradeScreenSwitchClock);
    free(state);
}
