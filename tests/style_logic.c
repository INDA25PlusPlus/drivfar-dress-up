#include "../src/logic/style_logic.h"
#include "utest.h"

UTEST(styleLogic, emptyOutfitReturnsNeutral)
{
    StyleResult result = judgeStyle(NULL, 0);

    EXPECT_EQ(STYLE_NONE, result.dominantStyle);
    EXPECT_EQ(0, result.dominantCount);
    EXPECT_EQ(0, result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, allNoneGarmentsReturnsNeutral)
{
    Garment outfit[] = {
        { .id = GARMENT_SHIRT },
    };

    StyleResult result = judgeStyle(outfit, 1);

    EXPECT_EQ(STYLE_NONE, result.dominantStyle);
    EXPECT_EQ(0, result.dominantCount);
    EXPECT_EQ(0, result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, singleStyledGarmentNoPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 1);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(1, result.dominantCount);
    EXPECT_EQ(1, result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, matchingPairEarnsPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
        { .id = GARMENT_FRACK_PANTS }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 2);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(2, result.dominantCount);
    EXPECT_EQ(1, result.styleCount);
    EXPECT_FALSE(result.hasClash);
    // Pair points (2) + coherent bonus (2) = 4
    EXPECT_EQ(4, result.stylePoints);
}

UTEST(styleLogic, threeMatchingGarmentsEarnsExtraPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
        { .id = GARMENT_FRACK_PANTS }, // STYLE_SITTNING
        { .id = GARMENT_DETAILS_FRACK }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 3);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(3, result.dominantCount);
    EXPECT_EQ(1, result.styleCount);
    EXPECT_FALSE(result.hasClash);
    // Pair points (2) + 1 extra match (1) + coherent bonus (2) = 5
    EXPECT_EQ(5, result.stylePoints);
}

UTEST(styleLogic, noCoherentBonusWhenMultipleStylesPresent)
{
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
        { .id = GARMENT_SNEAKERS }, // STYLE_OVERALL
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_EQ(2, result.styleCount);
    EXPECT_EQ(-3, result.stylePoints);
}

UTEST(styleLogic, finsittningClashesWithOverall)
{
    // SITTNING vs OVERALL must clash.
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
        { .id = GARMENT_SNEAKERS }, // STYLE_OVERALL
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_TRUE(result.hasClash);
}

UTEST(styleLogic, compatibleStylesDoNotClash)
{
    Garment outfit[] = {
        { .id = GARMENT_RAT }, // STYLE_NONE
        { .id = GARMENT_RAT }, // STYLE_NONE
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, dominantStyleIsTheMostFrequent)
{
    Garment outfit[] = {
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
        { .id = GARMENT_FRACK_PANTS }, // STYLE_SITTNING
        { .id = GARMENT_RAT }, // STYLE_NONE
    };

    StyleResult result = judgeStyle(outfit, 3);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(2, result.dominantCount);
}

UTEST(styleLogic, neutralGarmentsDoNotAffectDominantStyle)
{
    Garment outfit[] = {
        { .id = GARMENT_RAT }, // STYLE_NONE
        { .id = GARMENT_RAT }, // STYLE_NONE
        { .id = GARMENT_FRACK }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 3);

    // STYLE_NONE should never become the dominant style.
    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(1, result.styleCount);
}
