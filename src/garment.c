#include <CSFML/Graphics.h>
#include "garment.h"
#include "error_utilities.h"

sfColor colorToSfColor(PaletteColor color)
{
    switch (color) {
    case COLOR_WHITE:
        return (sfColor){ 0xff, 0xff, 0xff, 0xff };
    case COLOR_GRAY:
        return (sfColor){ 0x99, 0x99, 0x99, 0xff };
    case COLOR_BLACK:
        return (sfColor){ 0x00, 0x00, 0x00, 0xff };
    case COLOR_CERISE_STRONG:
        return (sfColor){ 0xff, 0x39, 0x7e, 0xff };
    case COLOR_CERISE_MEDIUM:
        return (sfColor){ 0xee, 0x88, 0x96, 0xff };
    case COLOR_CERISE_LIGHT:
        return (sfColor){ 0xf9, 0xdc, 0xdf, 0xff };
    case COLOR_ORANGE_STRONG:
        return (sfColor){ 0xd6, 0x7b, 0x00, 0xff };
    case COLOR_ORANGE_MEDIUM:
        return (sfColor){ 0xdb, 0x9d, 0x48, 0xff };
    case COLOR_ORANGE_LIGHT:
        return (sfColor){ 0xf2, 0xe2, 0xce, 0xff };
    case COLOR_GREEN_STRONG:
        return (sfColor){ 0x6e, 0xa9, 0x00, 0xff };
    case COLOR_GREEN_MEDIUM:
        return (sfColor){ 0x8d, 0xbb, 0x66, 0xff };
    case COLOR_GREEN_LIGHT:
        return (sfColor){ 0xdd, 0xe9, 0xd4, 0xff };
    case COLOR_CYAN_STRONG:
        return (sfColor){ 0x00, 0xad, 0xa6, 0xff };
    case COLOR_CYAN_MEDIUM:
        return (sfColor){ 0x14, 0xc3, 0xbf, 0xff };
    case COLOR_CYAN_LIGHT:
        return (sfColor){ 0xcd, 0xec, 0xea, 0xff };
    case COLOR_BLUE_STRONG:
        return (sfColor){ 0x00, 0x95, 0xff, 0xff };
    case COLOR_BLUE_MEDIUM:
        return (sfColor){ 0x6e, 0xaf, 0xf7, 0xff };
    case COLOR_BLUE_LIGHT:
        return (sfColor){ 0xd6, 0xe6, 0xfa, 0xff };
    case COLOR_PURPLE_STRONG:
        return (sfColor){ 0xc3, 0x5b, 0xf9, 0xff };
    case COLOR_PURPLE_MEDIUM:
        return (sfColor){ 0xc5, 0x93, 0xe4, 0xff };
    case COLOR_PURPLE_LIGHT:
        return (sfColor){ 0xec, 0xdf, 0xf4, 0xff };
    }

    ASSERT_UNREACHABLE();
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
    // should be white. May be NULL, in which case no color texture is rendered.
    const char *coloredPath;
    // Path to the image file containing any details which shouldn't be
    // affected by the selected color (e.g. outlines). May be NULL, in which
    // case no color texture is rendered.
    const char *detailsPath;
    // Position relative to doll
    sfVector2f position;
    // Scale the garmet image
    sfVector2f scale;
} GarmentDefinition;

// If you want to add a new garment asset: create a new enum member in
// `GarmentId`, and add a new entry in this array with that ID.
static GarmentDefinition garmentDefinitions[] = {
    [GARMENT_DETAILS_FRACK] =
        (GarmentDefinition){
            .id = GARMENT_DETAILS_FRACK,
            .type = GARMENT_TYPE_TOP,
            .name = "Frackband",
            .detailsPath = "assets/garment/details_frack/details.png",
            .position = { 1104.0, 914.0 },
            .scale = { 1.0, 1.0 },
        },
    [GARMENT_FRACK] =
        (GarmentDefinition){
            .id = GARMENT_FRACK,
            .type = GARMENT_TYPE_TOP,
            .name = "Frack",
            .coloredPath = "assets/garment/frack/colored.png",
            .detailsPath = "assets/garment/frack/details.png",
            .position = { 468.0, 660.0 },
            .scale = { 1.0, 1.0 },
        },
    [GARMENT_FRACK_PANTS] =
        (GarmentDefinition){
            .id = GARMENT_FRACK_PANTS,
            .type = GARMENT_TYPE_BOTTOM,
            .name = "Frack pants",
            .coloredPath = "assets/garment/frack_pants/colored.png",
            .detailsPath = "assets/garment/frack_pants/details.png",
            .position = { 893.0, 1570.0 },
            .scale = { 1.0, 1.0 },
        },
    [GARMENT_LOAFERS] =
        (GarmentDefinition){
            .id = GARMENT_LOAFERS,
            .type = GARMENT_TYPE_SHOES,
            .name = "Loafers",
            .coloredPath = "assets/garment/loafers/colored.png",
            .detailsPath = "assets/garment/loafers/details.png",
            .position = { 1004.0, 3801.0 },
            .scale = { 1.0, 1.0 },
        },
    [GARMENT_OVVE] =
        (GarmentDefinition){
            .id = GARMENT_OVVE,
            .type = GARMENT_TYPE_BOTTOM,
            .name = "Pants Ovve",
            .coloredPath = "assets/garment/ovve/colored.png",
            .detailsPath = "assets/garment/ovve/details.png",
            .position = { 880.0, 1426.0 },
            .scale = { 1.0f, 1.0f },
        },
    [GARMENT_RAT] =
        (GarmentDefinition){
            .id = GARMENT_RAT,
            .type = GARMENT_TYPE_HAT,
            .name = "Rat <3",
            .detailsPath = "assets/garment/rat/details.png",
            .position = { 1349.0, 598.0 },
            .scale = { 1.0f, 1.0f },
        },
    [GARMENT_SCHMECK] =
        (GarmentDefinition){
            .id = GARMENT_SCHMECK,
            .type = GARMENT_TYPE_HAT,
            .name = "Schmeck",
            .coloredPath = "assets/garment/schmeck/colored.png",
            .detailsPath = "assets/garment/schmeck/details.png",
            .position = { 950.0, 130.0 },
            .scale = { 1.0f, 1.0f },
        },
    [GARMENT_SHIRT] =
        (GarmentDefinition){
            .id = GARMENT_SHIRT,
            .type = GARMENT_TYPE_TOP,
            .name = "Shirt",
            .coloredPath = "assets/garment/shirt/colored.png",
            .detailsPath = "assets/garment/shirt/details.png",
            .position = { 771.0, 723.0 },
            .scale = { 1.0f, 1.0f },
        },
    [GARMENT_SNEAKERS] =
        (GarmentDefinition){
            .id = GARMENT_SNEAKERS,
            .type = GARMENT_TYPE_SHOES,
            .name = "Sneakers",
            .coloredPath = "assets/garment/sneakers/colored.png",
            .detailsPath = "assets/garment/sneakers/details.png",
            .position = { 1000.0, 3795.0 },
            .scale = { 1.0f, 1.0f },
        },
    [GARMENT_SCHLEMFILTER] =
        (GarmentDefinition){
            .id = GARMENT_SCHLEMFILTER,
            .type = GARMENT_TYPE_HAT,
            .name = "Schlemfilter",
            .detailsPath = "assets/garment/schlemfilter/details.png",
            .position = { 1008.0, 377.0 },
            .scale = { 1.0f, 1.0f },
        },
};

#define GARMENT_DEFINITIONS_COUNT \
    (sizeof(garmentDefinitions) / sizeof(GarmentDefinition))

GarmentAsset garments[GARMENT_COUNT];

bool loadGarments()
{
    static_assert(GARMENT_DEFINITIONS_COUNT == GARMENT_COUNT,
                  "Number of garment IDs must match number of garments");
    for (size_t i = 0; i < GARMENT_COUNT; i++) {
        GarmentDefinition definition = garmentDefinitions[i];

        sfTexture *coloredTexture = NULL;
        if (definition.coloredPath != NULL) {
            coloredTexture =
                sfTexture_createFromFile(definition.coloredPath, NULL);
            if (coloredTexture == NULL) {
                // Destroy the previously created textures.
                for (size_t j = 0; j < i; j++) {
                    sfTexture_destroy(garments[j].coloredTexture);
                    sfTexture_destroy(garments[j].detailsTexture);
                }
                return false;
            }
            sfTexture_generateMipmap(coloredTexture);
            sfTexture_setSmooth(coloredTexture, true);
        }

        sfTexture *detailsTexture = NULL;
        if (definition.detailsPath != NULL) {
            detailsTexture =
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
            sfTexture_generateMipmap(detailsTexture);
            sfTexture_setSmooth(detailsTexture, true);
        }

        garments[i] = (GarmentAsset){
            .id = definition.id,
            .type = definition.type,
            .name = definition.name,
            .coloredTexture = coloredTexture,
            .detailsTexture = detailsTexture,
            .position = definition.position,
            .scale = definition.scale,
        };
    }
    return true;
}
