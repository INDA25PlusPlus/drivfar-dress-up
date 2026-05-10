#ifndef STYLE_LOGIC_H
#define STYLE_LOGIC_H

#include <stdint.h>
#include <stddef.h>

#include "garment.h"

// The style category a garment belongs to.
// A garment may belong to exactly one style (or STYLE_NONE if it is neutral).
// Multiple garments sharing the same style reward the player; clashing styles
// penalize the player.
typedef enum {
    // The garment carries no particular style signal.
    STYLE_NONE,
    // Fulsittning (casual etc)
    STYLE_GASQUE,
    // Finsittning. (formal, tuxedo)
    STYLE_SITTNING,
    // Pub night: streetwear, statement pieces
    STYLE_PUB,
    // Stuff that goes well with an overall
    STYLE_OVERALL,
} GarmentStyle;

// Returns a human-readable Swedish name for a style (e.g. "Gasque").
const char *styleToString(GarmentStyle style);

// Returns the style associated with a garment, or STYLE_NONE if the garment
// carries no style tag.
GarmentStyle styleForGarment(GarmentId id);

// Describes the style composition of an outfit.
typedef struct {
    // The dominant style (the one with the most garments), or STYLE_NONE if
    // the outfit is neutral / evenly split.
    GarmentStyle dominantStyle;

    // How many garments belong to the dominant style.
    uint8_t dominantCount;

    // Number of distinct non-NONE styles present in the outfit.
    uint8_t styleCount;

    // Whether at least one pair of clashing styles is present.
    bool hasClash;

    // Net points contributed by style logic (can be negative due to clashes).
    int8_t stylePoints;
} StyleResult;

// Compute the style result for an array of `count` garments.
StyleResult judgeStyle(const Garment *garments, size_t count);

#endif // STYLE_LOGIC_H