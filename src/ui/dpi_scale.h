#ifndef DPI_SCALING_H
#define DPI_SCALING_H

/// How much the UI should be scaled by.
extern float uiScale;

/// Parse the UI scale from the `DRIFVARDRESSING_UI_SCALE` environment variable
/// and store it into the `uiScale` variable, defaulting to `1.0` if the
/// environment variable isn't present or doesn't contain a float.
///
/// Note: It would be nice to read the DPI scaling from the OS, but that doesn't
///   seem to be supported by SFML...
void loadUiScale();

#endif
