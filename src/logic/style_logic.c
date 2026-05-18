#include "style_logic.h"
#include "error_utilities.h"
#include <stdbool.h>

// TODO: Balance this with color scheme

const char *styleToString(GarmentStyle style)
{
    switch (style) {
    case STYLE_NONE:
        return "Neutral";
    case STYLE_GASQUE:
        return "Gasque";
    case STYLE_SITTNING:
        return "Finsittning";
    case STYLE_PUB:
        return "Pub";
    case STYLE_OVERALL:
        return "Overall";
    default:
        ASSERT_UNKNOWN_GARMENT_STYLE();
        return "Neutral"; // unreachable, silences compiler warning
    }
}

// Garment -> style mapping
//
// Add a case here whenever you add a new GarmentId.
GarmentStyle styleForGarment(GarmentId id)
{
    switch (id) {
    case GARMENT_LOAFERS:
        return STYLE_SITTNING;
    case GARMENT_SNEAKERS:
        return STYLE_PUB;
    case GARMENT_SHIRT:
        return STYLE_NONE;
    case GARMENT_OVVE:
        return STYLE_OVERALL;
    case GARMENT_FRACK:
        return STYLE_SITTNING;
    case GARMENT_FRACK_PANTS:
        return STYLE_SITTNING;
    case GARMENT_DETAILS_FRACK:
        return STYLE_SITTNING;
    case GARMENT_SCHLEMFILTER:
        return STYLE_NONE;
    case GARMENT_SCHMECK:
        return STYLE_SITTNING;
    case GARMENT_RAT:
        return STYLE_NONE;
    default:
        ASSERT_UNKNOWN_GARMENT_ID_STYLE();
    }
}

// Number of named styles (excluding STYLE_NONE).
#define STYLE_COUNT 4

// Clash table
//
// Returns true when two different non-NONE styles clash with each other.
// The table is symmetric: clash(A, B) == clash(B, A).
//
// Flat symmetric clash matrix indexed by (GarmentStyle - 1).
// 1 = clashes, 0 = compatible.
static const uint8_t clashMatrix[STYLE_COUNT][STYLE_COUNT] = {
    //         GAS SIT PUB OVR
    /* GAS */ { 0, 1, 0, 0 },
    /* SIT */ { 1, 0, 1, 1 },
    /* PUB */ { 0, 1, 0, 0 },
    /* OVR */ { 0, 1, 0, 0 },
};

static bool stylesClash(GarmentStyle a, GarmentStyle b)
{
    if (a == STYLE_NONE || b == STYLE_NONE)
        return false;
    if (a == b)
        return false;
    return clashMatrix[a - 1][b - 1] == 1;
}

// Scoring constants

// Points awarded for having exactly two garments of the same style.
#define STYLE_PAIR_POINTS 2
// Extra points awarded for each additional matching garment beyond two.
#define STYLE_EXTRA_MATCH_POINTS 1
// Bonus for a fully coherent outfit (all styled garments share one style).
#define STYLE_COHERENT_BONUS 2
// Penalty applied per clashing style pair detected.
#define STYLE_CLASH_PENALTY (-3)

StyleResult judgeStyle(const Garment *outfit, size_t count)
{
    // Tally how many garments belong to each style.
    // Index 0 = STYLE_NONE, indices 1..STYLE_COUNT = named styles.
    uint8_t tally[STYLE_COUNT + 1] = { 0 };

    for (size_t i = 0; i < count; i++) {
        GarmentStyle s = styleForGarment(outfit[i].id);
        if (s <= STYLE_OVERALL) {
            tally[(int)s]++;
        }
    }

    // Find the dominant style (highest count among non-NONE styles).
    GarmentStyle dominantStyle = STYLE_NONE;
    uint8_t dominantCount = 0;
    uint8_t styleCount = 0;

    for (int s = 1; s <= STYLE_COUNT; s++) {
        if (tally[s] > 0) {
            styleCount++;
            if (tally[s] > dominantCount) {
                dominantCount = tally[s];
                dominantStyle = (GarmentStyle)s;
            }
        }
    }

    // Collect which non-NONE styles are present, then detect clashes between
    // every pair.
    GarmentStyle present[STYLE_COUNT];
    int presentCount = 0;
    for (int s = 1; s <= STYLE_COUNT; s++) {
        if (tally[s] > 0) {
            present[presentCount++] = (GarmentStyle)s;
        }
    }

    bool hasClash = false;
    int clashPairs = 0;
    for (int i = 0; i < presentCount; i++) {
        for (int j = i + 1; j < presentCount; j++) {
            if (stylesClash(present[i], present[j])) {
                hasClash = true;
                clashPairs++;
            }
        }
    }

    // Calculate points.
    int8_t points = 0;

    if (dominantCount >= 2) {
        points += STYLE_PAIR_POINTS;
        points += (int8_t)((dominantCount - 2) * STYLE_EXTRA_MATCH_POINTS);
    }

    // Coherent outfit bonus: all styled garments share the same style.
    if (styleCount == 1 && dominantCount >= 2) {
        points += STYLE_COHERENT_BONUS;
    }

    // Clash penalty.
    points += (int8_t)(clashPairs * STYLE_CLASH_PENALTY);

    // Clamp to avoid ridiculous negative scores.
    if (points < -10)
        points = -10;

    return (StyleResult){
        .dominantStyle = dominantStyle,
        .dominantCount = dominantCount,
        .styleCount = styleCount,
        .hasClash = hasClash,
        .stylePoints = points,
    };
}
