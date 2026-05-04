#include "../src/logic/color_logic.h"
#include "utest.h"

UTEST(colorLogic, returnsNoneForSingleColor)
{
    PaletteColor colors[] = {
        COLOR_RED,
    };

    EXPECT_EQ(COLOR_SCHEME_NONE, judgeColorScheme(colors, 1));
}

UTEST(colorLogic, detectsComplementaryScheme)
{
    PaletteColor colors[] = {
        COLOR_RED,
        COLOR_GREEN,
    };

    EXPECT_EQ(COLOR_SCHEME_COMPLEMENTARY, judgeColorScheme(colors, 2));
}

UTEST(colorLogic, detectsAnalogousScheme)
{
    PaletteColor colors[] = {
        COLOR_RED,
        COLOR_RED_ORANGE,
        COLOR_RED_VIOLET,
    };

    EXPECT_EQ(COLOR_SCHEME_ANALOGOUS, judgeColorScheme(colors, 3));
}

UTEST(colorLogic, detectsTriadicScheme)
{
    PaletteColor colors[] = {
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_BLUE,
    };

    EXPECT_EQ(COLOR_SCHEME_TRIADIC, judgeColorScheme(colors, 3));
}

UTEST(colorLogic, detectsSplitComplementaryScheme)
{
    PaletteColor colors[] = {
        COLOR_RED,
        COLOR_YELLOW_GREEN,
        COLOR_BLUE_GREEN,
    };

    EXPECT_EQ(
        COLOR_SCHEME_SPLIT_COMPLEMENTARY,
        judgeColorScheme(colors, 3)
    );
}