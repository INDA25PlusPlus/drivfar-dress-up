#include "garment_list.h"

// The final grade after finishing the game.
typedef enum {
	GRADE_C,
	GRADE_B,
	GRADE_A,
	GRADE_S,
} grade_t;

// Contains all data used by the game logic.
typedef struct {
	garment_list_t garments;
} game_state_t;
