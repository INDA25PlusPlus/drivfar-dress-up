#include "garment_list.h"

// The final grade after finishing the game.
typedef enum {
    GRADE_C,
    GRADE_B,
    GRADE_A,
    GRADE_S,
} Grade;

// Contains all data used by the game logic.
typedef struct {
    GarmentList garments;
} GameState;
