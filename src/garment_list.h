#ifndef GARMENT_LIST_H
#define GARMENT_LIST_H

#include "garment.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

typedef struct {
	size_t len;
	size_t capacity;
	garment_t *items;
} garment_list_t;

garment_list_t *garment_list_create() {
	garment_list_t *list = malloc(sizeof(garment_list_t));
	if (!list) return NULL;
	list->len = 0

}
void garment_list_destroy(garment_list_t *list) {
	if (!list) return;
	free(list->items);
	free(list);
}
void garment_list_push(garment_list_t *list, garment_t garment) {
	assert(list);
	list->items[list->len++] = garment;
}
// Removes the specified index from the list, shifting the other elements after
// it one index down.
void garment_list_remove_index(garment_list_t *list, size_t index) {
	assert(list);
	assert(index < list->len);
	memmove(&list->items[index],&list->items[index+1],(list->len - index - 1) * sizeof(garment_t));
	list->len--;
}

#endif
