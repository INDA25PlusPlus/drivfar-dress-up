#include "../src/garment.h"
#include "../src/logic/color_logic.h"
#include "../src/logic/garment_color_helper.h"
#include "utest.h"

UTEST(garmentColorHelper, extractsColorsFromGarments)
{
    Garment garments[] = {
        {
            .id = GARMENT_SHIRT,
            .color = COLOR_CERISE_STRONG,
        },
        {
            .id = GARMENT_SNEAKERS,
            .color = COLOR_GREEN_STRONG,
        },
    };

    PaletteColor colors[2];

    size_t colorCount = garmentsToPaletteColors(
        garments,
        2,
        colors,
        2
    );

    EXPECT_EQ(2, colorCount);
    EXPECT_EQ(COLOR_CERISE_STRONG, colors[0]);
    EXPECT_EQ(COLOR_GREEN_STRONG, colors[1]);
}

UTEST(garmentColorHelper, passesExtractedColorsToJudgeColorScheme)
{
    Garment garments[] = {
        { .id = GARMENT_SHIRT,  .color = COLOR_CERISE_STRONG },
        { .id = GARMENT_SNEAKERS, .color = COLOR_CYAN_STRONG   },
    };

    PaletteColor colors[2];

    size_t colorCount = garmentsToPaletteColors(
        garments,
        2,
        colors,
        2
    );

    ColorScheme scheme = judgeColorScheme(colors, colorCount);

    EXPECT_EQ(COLOR_SCHEME_COMPLEMENTARY, scheme);
}