#include "grading_logic.h"

#include <stdbool.h>

#include "error_utilities.h"

// The choice for points is currently arbitrary (ish) and will likely have to remain arbitrary (ish)
//
// Color scheme scoring is based on how visually deliberate and "difficult" the
// scheme is to coordinate.
//
// Analogous colors are easy to combine because they are close together on the
// color wheel, so they receive a moderate score.
//
// Complementary and split-complementary colors create stronger contrast and
// are somewhat harder to balance, so they receive a higher score.
//
// Triadic color schemes use three evenly spaced colors. This can create a
// visually strong outfit, but is also easier fuck up. Because of
// that, triadic schemes currently receive the highest score.

uint8_t pointsForColorScheme(ColorScheme colorScheme)
{
    switch (colorScheme) {
    case COLOR_SCHEME_NONE:
        return 0;
    case COLOR_SCHEME_ANALOGOUS:
        return 3;
    case COLOR_SCHEME_COMPLEMENTARY:
        return 4;
    case COLOR_SCHEME_SPLIT_COMPLEMENTARY:
        return 4;
    case COLOR_SCHEME_TRIADIC:
        return 5;
    default:
        ASSERT_UNKNOWN_COLOR_SCHEME_GRADING();
        return 0;
    }
}

Grade gradeFromPoints(uint8_t points)
{
    if (points >= 5) {
        return GRADE_A;
    }

    if (points == 4) {
        return GRADE_B;
    }

    if (points == 3) {
        return GRADE_C;
    }

    if (points == 2) {
        return GRADE_D;
    }

    if (points == 1) {
        return GRADE_E;
    }

    return GRADE_F;
}

static bool hasCoherentStyleBonus(StyleResult styleResult)
{
    return styleResult.styleCount == 1 && styleResult.dominantCount >= 2;
}

static GradeBasis basisForCoherentStyle(GarmentStyle style)
{
    switch (style) {
    case STYLE_GASQUE:
        return GRADE_BASIS_STYLE_COHERENT_GASQUE;
    case STYLE_SITTNING:
        return GRADE_BASIS_STYLE_COHERENT_SITTNING;
    case STYLE_PUB:
        return GRADE_BASIS_STYLE_COHERENT_PUB;
    case STYLE_OVERALL:
        return GRADE_BASIS_STYLE_COHERENT_OVERALL;
    case STYLE_NONE:
        return GRADE_BASIS_NONE;
    default:
        ASSERT_UNKNOWN_GARMENT_STYLE_GRADING();
        return GRADE_BASIS_NONE;
    }
}

static GradeBasis basisForColorScheme(ColorScheme colorScheme)
{
    switch (colorScheme) {
    case COLOR_SCHEME_COMPLEMENTARY:
        return GRADE_BASIS_COLOR_COMPLEMENTARY;
    case COLOR_SCHEME_ANALOGOUS:
        return GRADE_BASIS_COLOR_ANALOGOUS;
    case COLOR_SCHEME_TRIADIC:
        return GRADE_BASIS_COLOR_TRIADIC;
    case COLOR_SCHEME_SPLIT_COMPLEMENTARY:
        return GRADE_BASIS_COLOR_SPLIT_COMPLEMENTARY;
    case COLOR_SCHEME_NONE:
        return GRADE_BASIS_NONE;
    default:
        ASSERT_UNKNOWN_COLOR_SCHEME_GRADING();
        return GRADE_BASIS_NONE;
    }
}

static GradeBasis basisForGrade(ColorScheme colorScheme,
                                StyleResult styleResult)
{
    if (styleResult.hasClash)
        return GRADE_BASIS_STYLE_CLASH;

    if (hasCoherentStyleBonus(styleResult))
        return basisForCoherentStyle(styleResult.dominantStyle);

    return basisForColorScheme(colorScheme);
}

GradeResult judgeGrade(ColorScheme colorScheme, StyleResult styleResult)
{
    uint8_t colorSchemePoints = pointsForColorScheme(colorScheme);
    int16_t total = (int16_t)colorSchemePoints + styleResult.stylePoints;
    if (total < 0)
        total = 0;

    return (GradeResult){
        .colorScheme = colorScheme,
        .colorSchemePoints = colorSchemePoints,
        .stylePoints = styleResult.stylePoints,
        .totalPoints = (uint8_t)total,
        .grade = gradeFromPoints((uint8_t)total),
        .basis = basisForGrade(colorScheme, styleResult),
    };
}
