#include <stdlib.h>

float uiScale = 1.0;

void loadUiScale()
{
    const char *uiScaleStr = getenv("DRIFVARDRESSING_UI_SCALE");
    if (uiScaleStr != 0) {
        char *readEnd;
        uiScale = strtof(uiScaleStr, &readEnd);
        if (readEnd != uiScaleStr) {
            // A float was successfully read from the environment variable.
            return;
        }
    }

    uiScale = 1.0;
}