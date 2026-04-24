#ifndef APPAREL_H
#define APPAREL_H

#include <stdint.h>
#include <CSFML/Graphics.h>

typedef enum {
    // Piece of clothing on the upper body, e.g. a shirt.
    GARMENT_TYPE_TOP,
    // Piece of clothing on the lower body, e.g. a pair of pants.
    GARMENT_TYPE_BOTTOM,
    GARMENT_TYPE_SHOES,
    GARMENT_TYPE_HAT,
} GarmentType;

// Hard coded list of garments, the order decides which order they are rendered
// in, with later items placed on top of earlier ones.
typedef enum {
    // The names should be prefixed by `GARMENT_`.
    GARMENT_TEST_A,
    GARMENT_TEST_B,
} GarmentId;

// Represents a piece of clothing or accessory.
typedef struct {
    GarmentId id;
    GarmentType type;
    // The user-facing name of the garment.
    const char *name;
    // Texture containing the colored layer of the garment, which should be
    // tinted by the selected color. Is never destroyed.
    const sfTexture *coloredTexture;
    // Texture containing the details layer of the garment, which shouldn't
    // be affected by the selected color. Is never destroyed. Is drawn on
    // top of the colored layer.
    const sfTexture *detailsTexture;
    // TODO: We will probably need more members to control where the asset
    //   is placed.
} GarmentAsset;

// The palette colors which the user can select for a garment.
typedef enum {
    COLOR_RED,
    COLOR_CERISE,
} PaletteColor;

// Returns the color of one of the palette colors.
sfColor colorToSfColor(PaletteColor color);

// A garment which has a configured color.
typedef struct {
    GarmentId id;
    PaletteColor color;
} Garment;

// The list of loaded garment assets. Is indexed using `GarmentId`.
//
// Must not be accessed before `loadGarments` has been called.
extern GarmentAsset garments[];

// Load garnet assets. Returns false if error is encountered.
bool loadGarments();

#endif
