#ifndef KEY_GRADE_H
#define KEY_GRADE_H

#include "ui/state.h"

/// Draws the modal asking the user for confirmation of submitting the design
/// for grading.
void GradeConfirmationModal();

/// Draws the score view.
void GradeView(UiState *state);

#endif
