#include "ui/color_grid.h"
#include "utest.h"

UTEST(colorGrid, getColorPositionCeriseStrong)
{
    sfVector2u result = colorGridGetPositionOfColor(COLOR_CERISE_STRONG);
    EXPECT_EQ(result.x, 0);
    EXPECT_EQ(result.y, 0);
}

UTEST(colorGrid, getColorPositionPurpleMedium)
{
    sfVector2u result = colorGridGetPositionOfColor(COLOR_PURPLE_MEDIUM);
    EXPECT_EQ(result.x, 5);
    EXPECT_EQ(result.y, 1);
}

UTEST(colorGrid, getColorAtPosition5_1)
{
    EXPECT_EQ(colorGridGetColorAtPosition((sfVector2u){ 5, 1 }),
              COLOR_PURPLE_MEDIUM);
}

UTEST(colorGrid, getAdjacentColorPurpleMedium)
{
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_PURPLE_MEDIUM,
                                        GRID_DIRECTION_LEFT),
              COLOR_BLUE_MEDIUM);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_PURPLE_MEDIUM,
                                        GRID_DIRECTION_ABOVE),
              COLOR_PURPLE_STRONG);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_PURPLE_MEDIUM,
                                        GRID_DIRECTION_RIGHT),
              COLOR_GRAY);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_PURPLE_MEDIUM,
                                        GRID_DIRECTION_BELOW),
              COLOR_PURPLE_LIGHT);
}

UTEST(colorGrid, getAdjacentColorAtEdge)
{
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_CERISE_STRONG,
                                        GRID_DIRECTION_LEFT),
              COLOR_CERISE_STRONG);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_CERISE_STRONG,
                                        GRID_DIRECTION_ABOVE),
              COLOR_CERISE_STRONG);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_WHITE,
                                        GRID_DIRECTION_RIGHT),
              COLOR_WHITE);
    EXPECT_EQ(colorGridGetAdjacentColor(COLOR_WHITE,
                                        GRID_DIRECTION_BELOW),
              COLOR_WHITE);
}
