#include "color_logic.h"

#define COLOR_WHEEL_SIZE 12
#define INVALID_COLOR_POSITION -1

static int32_t colorWheelPosition(PaletteColor color)
{
    switch (color) {
    case COLOR_CERISE_STRONG:
    case COLOR_CERISE_MEDIUM:
    case COLOR_CERISE_LIGHT:
        return 0;

    case COLOR_ORANGE_STRONG:
    case COLOR_ORANGE_MEDIUM:
    case COLOR_ORANGE_LIGHT:
        return 1;

    case COLOR_GREEN_STRONG:
    case COLOR_GREEN_MEDIUM:
    case COLOR_GREEN_LIGHT:
        return 2;

    case COLOR_CYAN_STRONG:
    case COLOR_CYAN_MEDIUM:
    case COLOR_CYAN_LIGHT:
        return 3;

    case COLOR_BLUE_STRONG:
    case COLOR_BLUE_MEDIUM:
    case COLOR_BLUE_LIGHT:
        return 4;

    case COLOR_PURPLE_STRONG:
    case COLOR_PURPLE_MEDIUM:
    case COLOR_PURPLE_LIGHT:
        return 5;

    case COLOR_WHITE:
    case COLOR_GRAY:
    case COLOR_BLACK:
        return INVALID_COLOR_POSITION;
    }

    return INVALID_COLOR_POSITION;
}

static bool getValidPosition(PaletteColor color, int32_t *outPosition)
{
    int32_t position = colorWheelPosition(color);

    if (position == INVALID_COLOR_POSITION) {
        return false;
    }

    *outPosition = position;
    return true;
}

static bool hasColorPosition(const PaletteColor colors[], size_t colorCount,
                             int32_t wantedPosition)
{
    for (size_t i = 0; i < colorCount; i++) {
        int32_t position;

        if (!getValidPosition(colors[i], &position)) {
            continue;
        }

        if (position == wantedPosition) {
            return true;
        }
    }

    return false;
}

static int32_t wrappedPosition(int32_t position)
{
    while (position < 0) {
        position += COLOR_WHEEL_SIZE;
    }

    return position % COLOR_WHEEL_SIZE;
}

static bool hasComplementaryScheme(const PaletteColor colors[],
                                   size_t colorCount)
{
    for (size_t i = 0; i < colorCount; i++) {
        int32_t position;

        if (!getValidPosition(colors[i], &position)) {
            continue;
        }

        int32_t oppositePosition = wrappedPosition(position + 3);

        if (hasColorPosition(colors, colorCount, oppositePosition)) {
            return true;
        }
    }

    return false;
}

static bool hasAnalogousScheme(const PaletteColor colors[], size_t colorCount)
{
    for (size_t i = 0; i < colorCount; i++) {
        int32_t position;

        if (!getValidPosition(colors[i], &position)) {
            continue;
        }

        int32_t leftPosition = wrappedPosition(position - 1);
        int32_t rightPosition = wrappedPosition(position + 1);

        if (hasColorPosition(colors, colorCount, leftPosition) &&
            hasColorPosition(colors, colorCount, rightPosition)) {
            return true;
        }
    }

    return false;
}

static bool hasTriadicScheme(const PaletteColor colors[], size_t colorCount)
{
    for (size_t i = 0; i < colorCount; i++) {
        int32_t position;

        if (!getValidPosition(colors[i], &position)) {
            continue;
        }

        int32_t secondPosition = wrappedPosition(position + 2);
        int32_t thirdPosition = wrappedPosition(position + 4);

        if (hasColorPosition(colors, colorCount, secondPosition) &&
            hasColorPosition(colors, colorCount, thirdPosition)) {
            return true;
        }
    }

    return false;
}

static bool hasSplitComplementaryScheme(const PaletteColor colors[],
                                        size_t colorCount)
{
    for (size_t i = 0; i < colorCount; i++) {
        int32_t position;

        if (!getValidPosition(colors[i], &position)) {
            continue;
        }

        int32_t oppositePosition = wrappedPosition(position + 3);
        int32_t leftSplitPosition = wrappedPosition(oppositePosition - 1);
        int32_t rightSplitPosition = wrappedPosition(oppositePosition + 1);

        if (hasColorPosition(colors, colorCount, leftSplitPosition) &&
            hasColorPosition(colors, colorCount, rightSplitPosition)) {
            return true;
        }
    }

    return false;
}

ColorScheme judgeColorScheme(const PaletteColor colors[], size_t colorCount)
{
    if (colorCount < 2) {
        return COLOR_SCHEME_NONE;
    }

    if (hasTriadicScheme(colors, colorCount)) {
        return COLOR_SCHEME_TRIADIC;
    }

    if (hasSplitComplementaryScheme(colors, colorCount)) {
        return COLOR_SCHEME_SPLIT_COMPLEMENTARY;
    }

    if (hasComplementaryScheme(colors, colorCount)) {
        return COLOR_SCHEME_COMPLEMENTARY;
    }

    if (hasAnalogousScheme(colors, colorCount)) {
        return COLOR_SCHEME_ANALOGOUS;
    }

    return COLOR_SCHEME_NONE;
}