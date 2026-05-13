#include "doll.h"
#include "garment.h"
#include "ui/custom_element.h"
#include "ui/ui.h"
#include "ui/state.h"

void renderCustomElementDoll(sfRenderWindow *window, const UiState *uiState,
                             Clay_BoundingBox bounds)
{
    // Clear existing garments.
    g_doll->garments->len = 0;

    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        if (uiState->garmentsActive[id]) {
            garmentListPush(g_doll->garments,
                            (Garment){ .id = id,
                                       .color = uiState->garmentsColor[id] });
        }
    }

    sfVector2f position = (sfVector2f){
        bounds.x + bounds.width / 2,
        bounds.y + bounds.height / 2,
    };
    renderDoll(window, g_doll, position, bounds.height);
}
