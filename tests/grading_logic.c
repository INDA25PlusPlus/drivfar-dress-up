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

UTEST(gradingLogic, convertsThreePointsToC)
{
    EXPECT_EQ(GRADE_C, gradeFromPoints(3));
}

UTEST(gradingLogic, convertsFivePointsToA)
{
    EXPECT_EQ(GRADE_A, gradeFromPoints(5));
}

UTEST(gradingLogic, judgesGradeFromColorScheme)
{
    GradeResult result =
        judgeGradeFromColorScheme(COLOR_SCHEME_TRIADIC);

    EXPECT_EQ(5, result.colorSchemePoints);
    EXPECT_EQ(5, result.totalPoints);
    EXPECT_EQ(GRADE_A, result.grade);
}