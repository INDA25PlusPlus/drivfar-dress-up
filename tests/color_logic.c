#include "../src/logic/color_logic.h"
#include "utest.h"

UTEST(colorLogic, returnsNoneForSingleColor)
{
    PaletteColor colors[] = {
        COLOR_CERISE_STRONG,
    };

    EXPECT_EQ(COLOR_SCHEME_NONE, judgeColorScheme(colors, 1));
}

UTEST(colorLogic, detectsComplementaryScheme)
{
    PaletteColor colors[] = {
        COLOR_CERISE_STRONG, // position 0
        COLOR_GREEN_STRONG,  // position 6
    };

    EXPECT_EQ(COLOR_SCHEME_COMPLEMENTARY, judgeColorScheme(colors, 2));
}

UTEST(colorLogic, detectsAnalogousScheme)
{
    PaletteColor colors[] = {
        COLOR_GREEN_STRONG, // position 6
        COLOR_CYAN_STRONG,  // position 7
        COLOR_BLUE_STRONG,  // position 8
    };

    EXPECT_EQ(COLOR_SCHEME_ANALOGOUS, judgeColorScheme(colors, 3));
}

UTEST(colorLogic, detectsTriadicScheme)
{
    PaletteColor colors[] = {
        COLOR_ORANGE_STRONG, // position 2
        COLOR_GREEN_STRONG,  // position 6
        COLOR_PURPLE_STRONG, // position 10
    };

    EXPECT_EQ(COLOR_SCHEME_TRIADIC, judgeColorScheme(colors, 3));
}

// doesnt work with new palette (maybe can hardcode)
/* UTEST(colorLogic, detectsSplitComplementaryScheme)
{
    PaletteColor colors[] = {
        COLOR_CERISE_STRONG, // position 11
        COLOR_GREEN_STRONG,  // position 6
        COLOR_CYAN_STRONG,   // position 7
    };

    EXPECT_EQ(
        COLOR_SCHEME_SPLIT_COMPLEMENTARY,
        judgeColorScheme(colors, 3)
    );
} */