#include <CSFML/Graphics.h>
#include "garment.h"

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
	// TODO: We will probably need more properties to control where the
	//   asset is placed.
} garment_definition_t;

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
