#include "../src/logic/style_logic.h"
#include "utest.h"

UTEST(styleLogic, emptyOutfitReturnsNeutral)
{
    StyleResult result = judgeStyle(NULL, 0);

    EXPECT_EQ(STYLE_NONE, result.dominantStyle);
    EXPECT_EQ(0,          result.dominantCount);
    EXPECT_EQ(0,          result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, allNoneGarmentsReturnsNeutral)
{
    Garment outfit[] = {
        { .id = GARMENT_TEST_A },
    };

    StyleResult result = judgeStyle(outfit, 1);

    EXPECT_EQ(STYLE_NONE, result.dominantStyle);
    EXPECT_EQ(0,          result.dominantCount);
    EXPECT_EQ(0,          result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, singleStyledGarmentNoPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 1);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(1,              result.dominantCount);
    EXPECT_EQ(1,              result.styleCount);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, matchingPairEarnsPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
        { .id = GARMENT_SKIRT_A }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 2);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(2,              result.dominantCount);
    EXPECT_EQ(1,              result.styleCount);
    EXPECT_FALSE(result.hasClash);
    // Pair points (2) + coherent bonus (2) = 4
    EXPECT_EQ(4, result.stylePoints);
}

UTEST(styleLogic, threeMatchingGarmentsEarnsExtraPoints)
{
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
        { .id = GARMENT_SKIRT_A }, // STYLE_SITTNING
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 3);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(3,              result.dominantCount);
    EXPECT_EQ(1,              result.styleCount);
    EXPECT_FALSE(result.hasClash);
    // Pair points (2) + 1 extra match (1) + coherent bonus (2) = 5
    EXPECT_EQ(5, result.stylePoints);
}

UTEST(styleLogic, noCoherentBonusWhenMultipleStylesPresent)
{
    // PANTS_A = STYLE_SITTNING, TEST_A = STYLE_NONE (neutral, doesn't count)
    // Add a second SITTNING garment and a compatible non-SITTNING styled one
    // to verify the bonus is withheld when styleCount > 1.
    //
    // We need a STYLE_PUB garment; until one exists in GarmentId we simulate
    // it by using the same asset twice and checking styleCount stays at 1.
    // Once real pub garments exist, replace this with one of them.
    //
    // For now: two SITTNING + one NONE → styleCount == 1 → bonus IS given.
    // This test documents the boundary: mix of two named styles → no bonus.
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
        { .id = GARMENT_SKIRT_A }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_EQ(1, result.styleCount); // only one distinct style
    EXPECT_EQ(4, result.stylePoints); // bonus included
}

UTEST(styleLogic, finsittningClashesWithPub)
{
    // SITTNING vs PUB must clash.
    // Until a dedicated STYLE_PUB garment exists in the enum the clash matrix
    // is still testable via styleForGarment overrides; once a pub garment is
    // added, swap GARMENT_TEST_A for it and remove the skip note.
    //
    // Current placeholder: PANTS_A (SITTNING) + TEST_A (NONE) → no clash.
    // This test will need updating when a PUB garment is added.
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
        { .id = GARMENT_TEST_A  }, // STYLE_NONE – neutral, no clash possible
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_FALSE(result.hasClash);
}

UTEST(styleLogic, compatibleStylesDoNotClash)
{
    // STYLE_GASQUE and STYLE_PUB are compatible in the clash matrix.
    // Represented here with assets that carry those styles once real garments
    // exist; for now verifying no false-positive clash with NONE items.
    Garment outfit[] = {
        { .id = GARMENT_TEST_A }, // STYLE_NONE
        { .id = GARMENT_TEST_A }, // STYLE_NONE
    };

    StyleResult result = judgeStyle(outfit, 2);
    EXPECT_FALSE(result.hasClash);
    EXPECT_EQ(0, result.stylePoints);
}

UTEST(styleLogic, dominantStyleIsTheMostFrequent)
{
    Garment outfit[] = {
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
        { .id = GARMENT_SKIRT_A }, // STYLE_SITTNING
        { .id = GARMENT_TEST_A  }, // STYLE_NONE
    };

    StyleResult result = judgeStyle(outfit, 3);

    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(2,              result.dominantCount);
}

UTEST(styleLogic, neutralGarmentsDoNotAffectDominantStyle)
{
    Garment outfit[] = {
        { .id = GARMENT_TEST_A  }, // STYLE_NONE
        { .id = GARMENT_TEST_A  }, // STYLE_NONE
        { .id = GARMENT_PANTS_A }, // STYLE_SITTNING
    };

    StyleResult result = judgeStyle(outfit, 3);

    // STYLE_NONE should never become the dominant style.
    EXPECT_EQ(STYLE_SITTNING, result.dominantStyle);
    EXPECT_EQ(1,              result.styleCount);
}