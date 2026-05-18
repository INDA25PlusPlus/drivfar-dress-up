#ifndef GRADING_LOGIC_H
#define GRADING_LOGIC_H

#include <stddef.h>
#include <stdint.h>

#include "color_logic.h"
#include "style_logic.h"

typedef enum {
    GRADE_F,
    GRADE_E,
    GRADE_D,
    GRADE_C,
    GRADE_B,
    GRADE_A,
} Grade;

typedef enum {
    GRADE_BASIS_NONE,
    GRADE_BASIS_COLOR_COMPLEMENTARY,
    GRADE_BASIS_COLOR_ANALOGOUS,
    GRADE_BASIS_COLOR_TRIADIC,
    GRADE_BASIS_COLOR_SPLIT_COMPLEMENTARY,
    GRADE_BASIS_STYLE_COHERENT_SITTNING,
    GRADE_BASIS_STYLE_COHERENT_OVERALL,
    GRADE_BASIS_STYLE_CLASH,
    GRADE_BASIS_DRIFVARE_FIN,
} GradeBasis;

typedef struct {
    ColorScheme colorScheme;
    uint8_t colorSchemePoints;
    uint8_t totalPoints;
    int8_t stylePoints;
    Grade grade;
    GradeBasis basis;
} GradeResult;

uint8_t pointsForColorScheme(ColorScheme colorScheme);

Grade gradeFromPoints(uint8_t points);

GradeResult judgeGrade(ColorScheme colorScheme, StyleResult styleResult);

GradeResult judgeGradeForOutfit(ColorScheme colorScheme,
                                StyleResult styleResult,
                                const Garment outfit[], size_t outfitCount);

#endif
