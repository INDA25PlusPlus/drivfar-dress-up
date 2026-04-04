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
} garment_type_t;

// Hard coded list of garments, the order decides which order they are rendered
// in, with later items placed on top of earlier ones.
typedef enum {
	// The names should be prefixed by `GARMENT_`.
	GARMENT_TEST_A,
	GARMENT_TEST_B,
} garment_id_t;

// Represents a piece of clothing or accessory.
typedef struct {
	garment_id_t id;
	garment_type_t type;
	// The user-facing name of the garment.
	const char *name;
	// Texture containing the colored layer of the garment, which should be
	// tinted by the selected color. Is never destroyed.
	const sfTexture *colored_texture;
	// Texture containing the details layer of the garment, which shouldn't
	// be affected by the selected color. Is never destroyed. Is drawn on
	// top of the colored layer.
	const sfTexture *details_texture;
	// TODO: We will probably need more properties to control where the
	//   asset is placed.
} garment_asset_t;

typedef enum {
	COLOR_RED,
	COLOR_CERISE,
} color_t;

typedef struct {
	garment_id_t id;
	color_t color;
} garment_t;

// The list of loaded garment assets. Is indexed using `garment_id_t`.
//
// Must not be accessed before `load_garments` has been called.
extern garment_asset_t garments[];

// Load garnet assets. Returns false if error is encountered.
bool load_garments();

#endif
