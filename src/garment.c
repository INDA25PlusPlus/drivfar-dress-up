#include <CSFML/Graphics.h>
#include "garment.h"

sfColor color_to_sf_color(color_t color)
{
	switch (color) {
	case COLOR_RED:
		return (sfColor){ 0xe8, 0x3d, 0x40, 0xff };
	case COLOR_CERISE:
		return (sfColor){ 0xe8, 0x3d, 0x84, 0xff };
	}
}

// Version defining a garment asset used for the hard-coded
// `garment_definitions` array. This stores the graphics as paths, in contrast
// to `garment_asset_t`, which contains the loaded textures objects.
typedef struct {
	garment_id_t id;
	garment_type_t type;
	// The user-facing name of the garment.
	const char *name;
	// Path to the image file containing the parts of the garment which
	// should be tinted by the selected color. This image's base color
	// should be white.
	const char *colored_path;
	// Path to the image file containing any details which shouldn't be
	// affected by the selected color (e.g. outlines).
	const char *details_path;
	// TODO: We will probably need more members to control where the asset
	//   is placed.
} garment_definition_t;

// If you want to add a new garment asset: create a new enum member in
// `garnet_id_t`, and add a new entry in this array with that ID.
static garment_definition_t garment_definitions[] = {
	[GARMENT_TEST_A] =
		(garment_definition_t){
			.id = GARMENT_TEST_A,
			.type = GARMENT_TYPE_TOP,
			.name = "Test A",
			.colored_path = "assets/garment/test_a/colored.png",
			.details_path = "assets/garment/test_a/details.png",
		},
};

#define GARMENT_COUNT \
	(sizeof(garment_definitions) / sizeof(garment_definition_t))

garment_asset_t garments[GARMENT_COUNT];

bool load_garments()
{
	for (size_t i = 0; i < GARMENT_COUNT; i++) {
		garment_definition_t definition = garment_definitions[i];

		sfTexture *colored_texture =
			sfTexture_createFromFile(definition.colored_path, NULL);
		if (colored_texture == NULL) {
			// Destroy the previously created textures.
			for (size_t j = 0; j < i; j++) {
				sfTexture_destroy(garments[j].colored_texture);
				sfTexture_destroy(garments[j].details_texture);
			}
			return false;
		}

		sfTexture *details_texture =
			sfTexture_createFromFile(definition.details_path, NULL);
		if (details_texture == NULL) {
			sfTexture_destroy(colored_texture);
			// Destroy the previously created textures.
			for (size_t j = 0; j < i; j++) {
				sfTexture_destroy(garments[j].colored_texture);
				sfTexture_destroy(garments[j].details_texture);
			}
			return false;
		}

		garments[i] = (garment_asset_t){
			.id = definition.id,
			.type = definition.type,
			.name = definition.name,
			.colored_texture = colored_texture,
			.details_texture = details_texture,
		};
	}
	return true;
}
