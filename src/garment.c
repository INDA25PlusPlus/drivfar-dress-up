#include <CSFML/Graphics.h>
#include "garment.h"

sfColor colorToSfColor(PaletteColor color)
{
    switch (color) {
    case COLOR_RED:
        return (sfColor){ 0xe8, 0x3d, 0x40, 0xff };
    case COLOR_CERISE:
        return (sfColor){ 0xe8, 0x3d, 0x84, 0xff };
    }
}

// Version defining a garment asset used for the hard-coded
// `garmentDefinitions` array. This stores the graphics as paths, in contrast
// to `GarmentAsset`, which contains the loaded textures objects.
typedef struct {
    GarmentId id;
    GarmentType type;
    // The user-facing name of the garment.
    const char *name;
    // Path to the image file containing the parts of the garment which
    // should be tinted by the selected color. This image's base color
    // should be white.
    const char *coloredPath;
    // Path to the image file containing any details which shouldn't be
    // affected by the selected color (e.g. outlines).
    const char *detailsPath;
    // TODO: We will probably need more members to control where the asset
    //   is placed.
} GarmentDefinition;

// If you want to add a new garment asset: create a new enum member in
// `GarmentId`, and add a new entry in this array with that ID.
static GarmentDefinition garmentDefinitions[] = {
    [GARMENT_TEST_A] =
        (GarmentDefinition){
            .id = GARMENT_TEST_A,
            .type = GARMENT_TYPE_TOP,
            .name = "Test A",
            .coloredPath = "assets/garment/test_a/colored.png",
            .detailsPath = "assets/garment/test_a/details.png",
        },
    [GARMENT_PANTS_A] =
        (GarmentDefinition){
            .id = GARMENT_PANTS_A,
            .type = GARMENT_TYPE_BOTTOM,
            .name = "Pants A",
            .coloredPath = "assets/garment/pants_a/colored.png",
            .detailsPath = "assets/garment/pants_a/details.png",
        },
    [GARMENT_SKIRT_A] =
        (GarmentDefinition){
            .id = GARMENT_SKIRT_A,
            .type = GARMENT_TYPE_BOTTOM,
            .name = "Skirt A",
            .coloredPath = "assets/garment/skirt_a/colored.png",
            .detailsPath = "assets/garment/skirt_a/details.png",
        },
};

#define GARMENT_COUNT (sizeof(garmentDefinitions) / sizeof(GarmentDefinition))

GarmentAsset garments[GARMENT_COUNT];

bool loadGarments()
{
    for (size_t i = 0; i < GARMENT_COUNT; i++) {
        GarmentDefinition definition = garmentDefinitions[i];

        sfTexture *coloredTexture =
            sfTexture_createFromFile(definition.coloredPath, NULL);
        if (coloredTexture == NULL) {
            // Destroy the previously created textures.
            for (size_t j = 0; j < i; j++) {
                sfTexture_destroy(garments[j].coloredTexture);
                sfTexture_destroy(garments[j].detailsTexture);
            }
            return false;
        }

        sfTexture *detailsTexture =
            sfTexture_createFromFile(definition.detailsPath, NULL);
        if (detailsTexture == NULL) {
            sfTexture_destroy(coloredTexture);
            // Destroy the previously created textures.
            for (size_t j = 0; j < i; j++) {
                sfTexture_destroy(garments[j].coloredTexture);
                sfTexture_destroy(garments[j].detailsTexture);
            }
            return false;
        }

        garments[i] = (GarmentAsset){
            .id = definition.id,
            .type = definition.type,
            .name = definition.name,
            .coloredTexture = coloredTexture,
            .detailsTexture = detailsTexture,
        };
    }
    return true;
}
