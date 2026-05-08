#ifndef GRADING_LOGIC_H
#define GRADING_LOGIC_H

#include <stdint.h>

#include "color_logic.h"

typedef enum {
    GRADE_F,
    GRADE_E,
    GRADE_D,
    GRADE_C,
    GRADE_B,
    GRADE_A,
} Grade;

typedef struct {
    ColorScheme colorScheme;
    uint8_t colorSchemePoints;
    uint8_t totalPoints;
    Grade grade;
} GradeResult;

uint8_t pointsForColorScheme(ColorScheme colorScheme);

Grade gradeFromPoints(uint8_t points);

GradeResult judgeGradeFromColorScheme(ColorScheme colorScheme);

#endif