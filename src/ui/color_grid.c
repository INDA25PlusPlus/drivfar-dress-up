#include <assert.h>
#include <CSFML/Graphics.h>

#include "color_grid.h"
#include "error_utilities.h"
#include "garment.h"
#include "grid_direction.h"

static const PaletteColor colorGrid[COLOR_GRID_HEIGHT][COLOR_GRID_WIDTH] = {
    {
        COLOR_CERISE_STRONG,
        COLOR_ORANGE_STRONG,
        COLOR_GREEN_STRONG,
        COLOR_CYAN_STRONG,
        COLOR_BLUE_STRONG,
        COLOR_PURPLE_STRONG,
        COLOR_BLACK,
    },
    {
        COLOR_CERISE_MEDIUM,
        COLOR_ORANGE_MEDIUM,
        COLOR_GREEN_MEDIUM,
        COLOR_CYAN_MEDIUM,
        COLOR_BLUE_MEDIUM,
        COLOR_PURPLE_MEDIUM,
        COLOR_GRAY,
    },
    {
        COLOR_CERISE_LIGHT,
        COLOR_ORANGE_LIGHT,
        COLOR_GREEN_LIGHT,
        COLOR_CYAN_LIGHT,
        COLOR_BLUE_LIGHT,
        COLOR_PURPLE_LIGHT,
        COLOR_WHITE,
    },
};

sfVector2u colorGridGetPositionOfColor(PaletteColor color)
{
    for (unsigned int y = 0; y < COLOR_GRID_HEIGHT; y++) {
        for (unsigned int x = 0; x < COLOR_GRID_WIDTH; x++) {
            if (colorGrid[y][x] == color) {
                return (sfVector2u){ x, y };
            }
        }
    }

    ASSERT_UNREACHABLE_MSG("`color` should exist in grid");
}

PaletteColor colorGridGetColorAtPosition(sfVector2u position)
{
    assert(position.x < COLOR_GRID_WIDTH);
    assert(position.y < COLOR_GRID_HEIGHT);

    return colorGrid[position.y][position.x];
}

PaletteColor colorGridGetAdjacentColor(PaletteColor color,
                                       GridDirection direction)
{
    sfVector2u position = colorGridGetPositionOfColor(color);
    switch (direction) {
    case GRID_DIRECTION_LEFT: {
        if (position.x == 0) {
            return color;
        }
        return colorGridGetColorAtPosition(
            (sfVector2u){ position.x - 1, position.y });
    } break;
    case GRID_DIRECTION_ABOVE: {
        if (position.y == 0) {
            return color;
        }
        return colorGridGetColorAtPosition(
            (sfVector2u){ position.x, position.y - 1 });
    } break;
    case GRID_DIRECTION_RIGHT: {
        if (position.x == COLOR_GRID_WIDTH - 1) {
            return color;
        }
        return colorGridGetColorAtPosition(
            (sfVector2u){ position.x + 1, position.y });
    } break;
    case GRID_DIRECTION_BELOW: {
        if (position.y == COLOR_GRID_HEIGHT - 1) {
            return color;
        }
        return colorGridGetColorAtPosition(
            (sfVector2u){ position.x, position.y + 1 });
    } break;
    default:
        ASSERT_UNREACHABLE();
    }
}
