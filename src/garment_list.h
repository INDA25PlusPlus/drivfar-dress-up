#ifndef GARMENT_LIST_H
#define GARMENT_LIST_H

#include "garment.h"

typedef struct {
	size_t len;
	size_t capacity;
	garment_t *items;
} garment_list_t;

garment_list_t *garment_list_create();
void garment_list_destroy(garment_list_t *list);
void garment_list_push(garment_list_t *list, garment_t garment);
// Removes the specified index from the list, shifting the other elements after
// it one index down.
void garment_list_remove_index(garment_list_t *list, size_t index);

#endif
