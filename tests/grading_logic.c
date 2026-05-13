#include "../src/logic/grading_logic.h"
#include "utest.h"

UTEST(gradingLogic, givesZeroPointsForNoScheme)
{
    EXPECT_EQ(0, pointsForColorScheme(COLOR_SCHEME_NONE));
}

UTEST(gradingLogic, givesThreePointsForAnalogous)
{
    EXPECT_EQ(3, pointsForColorScheme(COLOR_SCHEME_ANALOGOUS));
}

UTEST(gradingLogic, givesFourPointsForComplementary)
{
    EXPECT_EQ(4, pointsForColorScheme(COLOR_SCHEME_COMPLEMENTARY));
}

UTEST(gradingLogic, givesFourPointsForSplitComplementary)
{
    EXPECT_EQ(4, pointsForColorScheme(COLOR_SCHEME_SPLIT_COMPLEMENTARY));
}

UTEST(gradingLogic, givesFivePointsForTriadic)
{
    EXPECT_EQ(5, pointsForColorScheme(COLOR_SCHEME_TRIADIC));
}

UTEST(gradingLogic, convertsZeroPointsToF)
{
    EXPECT_EQ(GRADE_F, gradeFromPoints(0));
}

UTEST(gradingLogic, convertsOnePointToE)
{
    EXPECT_EQ(GRADE_E, gradeFromPoints(1));
}

UTEST(gradingLogic, convertsTwoPointsToD)
{
    EXPECT_EQ(GRADE_D, gradeFromPoints(2));
}

UTEST(gradingLogic, convertsThreePointsToC)
{
    EXPECT_EQ(GRADE_C, gradeFromPoints(3));
}

UTEST(gradingLogic, convertsFourPointsToB)
{
    EXPECT_EQ(GRADE_B, gradeFromPoints(4));
}

UTEST(gradingLogic, convertsFivePointsToA)
{
    EXPECT_EQ(GRADE_A, gradeFromPoints(5));
}

UTEST(gradingLogic, convertsHighPointsToA)
{
    EXPECT_EQ(GRADE_A, gradeFromPoints(99));
}


// Helper: a neutral StyleResult that contributes zero style points.
static StyleResult neutralStyle(void)
{
    return (StyleResult){
        .dominantStyle = STYLE_NONE,
        .dominantCount = 0,
        .styleCount    = 0,
        .hasClash      = false,
        .stylePoints   = 0,
    };
}

UTEST(gradingLogic, neutralStyleDoesNotChangeColorPoints)
{
    GradeResult result = judgeGrade(COLOR_SCHEME_TRIADIC, neutralStyle());

    EXPECT_EQ(5,       result.colorSchemePoints);
    EXPECT_EQ(0,       result.stylePoints);
    EXPECT_EQ(5,       result.totalPoints);
    EXPECT_EQ(GRADE_A, result.grade);
    EXPECT_EQ(GRADE_BASIS_COLOR_TRIADIC, result.basis);
}

UTEST(gradingLogic, positiveStylePointsAddToTotal)
{
    StyleResult style = {
        .dominantStyle = STYLE_SITTNING,
        .dominantCount = 2,
        .styleCount    = 1,
        .hasClash      = false,
        .stylePoints   = 4, // pair (2) + coherent bonus (2)
    };

    // COLOR_SCHEME_ANALOGOUS = 3 points + 4 style points = 7 -> GRADE_A
    GradeResult result = judgeGrade(COLOR_SCHEME_ANALOGOUS, style);

    EXPECT_EQ(3,       result.colorSchemePoints);
    EXPECT_EQ(4,       result.stylePoints);
    EXPECT_EQ(7,       result.totalPoints);
    EXPECT_EQ(GRADE_A, result.grade);
    EXPECT_EQ(GRADE_BASIS_STYLE_COHERENT_SITTNING, result.basis);
}

UTEST(gradingLogic, negativeStylePointsReduceTotal)
{
    StyleResult style = {
        .dominantStyle = STYLE_SITTNING,
        .dominantCount = 1,
        .styleCount    = 2,
        .hasClash      = true,
        .stylePoints   = -3, // one clash penalty
    };

    // COLOR_SCHEME_COMPLEMENTARY = 4 points - 3 style points = 1 -> GRADE_E
    GradeResult result = judgeGrade(COLOR_SCHEME_COMPLEMENTARY, style);

    EXPECT_EQ(4,       result.colorSchemePoints);
    EXPECT_EQ(-3,      result.stylePoints);
    EXPECT_EQ(1,       result.totalPoints);
    EXPECT_EQ(GRADE_E, result.grade);
    EXPECT_EQ(GRADE_BASIS_STYLE_CLASH, result.basis);
}

UTEST(gradingLogic, totalPointsClampToZero)
{
    StyleResult style = {
        .dominantStyle = STYLE_NONE,
        .dominantCount = 0,
        .styleCount    = 2,
        .hasClash      = true,
        .stylePoints   = -10, // heavy clash penalty
    };

    // COLOR_SCHEME_NONE = 0 points - 10 style points -> clamped to 0 -> GRADE_F
    GradeResult result = judgeGrade(COLOR_SCHEME_NONE, style);

    EXPECT_EQ(0,       result.colorSchemePoints);
    EXPECT_EQ(-10,     result.stylePoints);
    EXPECT_EQ(0,       result.totalPoints);
    EXPECT_EQ(GRADE_F, result.grade);
    EXPECT_EQ(GRADE_BASIS_STYLE_CLASH, result.basis);
}

UTEST(gradingLogic, noBasisFallsBackToDefaultGradeMessage)
{
    GradeResult result = judgeGrade(COLOR_SCHEME_NONE, neutralStyle());

    EXPECT_EQ(GRADE_BASIS_NONE, result.basis);
}
