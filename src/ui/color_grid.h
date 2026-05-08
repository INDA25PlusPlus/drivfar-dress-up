#ifndef COLOR_GRID_H
#define COLOR_GRID_H

#include <CSFML/Graphics.h>

#include "garment.h"
#include "grid_direction.h"

#define COLOR_GRID_WIDTH 7
#define COLOR_GRID_HEIGHT 3

/// Get the position of the given color in the interactive color grid. The
/// positive directions go to the left and down.
/// @param color The color must exist in the color grid. (This is technically
///   not guaranteed, although we shouldn't add colors without adding them to
///   the grid.)
sfVector2u colorGridGetPositionOfColor(PaletteColor color);

/// Get the palette color which corresponds to the given position in the
/// interactive color grid. The positive directions go to the left and down.
/// @param position The `x` and `y` coordinates must be smaller than
///   `COLOR_GRID_WIDTH` and `COLOR_GRID_HEIGHT` respectively.
PaletteColor colorGridGetColorAtPosition(sfVector2u position);

/// Get one of the palette color which is adjacent to the given color in the
/// color grid.
/// @param color The color whose neighbor is returned. If it is at the edge in
///   the given direction then this color is returned instead.
/// @param direction Which of the four neighbors to return.
/// @return
PaletteColor colorGridGetAdjacentColor(PaletteColor color,
                                       GridDirection direction);

#endif