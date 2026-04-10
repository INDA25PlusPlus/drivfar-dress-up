#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "garment.h"
#include "garment_list.h"

garment_list_t *garment_list_create()
{
	return (garment_list_t *)calloc(1, sizeof(garment_list_t));
}

void garment_list_destroy(garment_list_t *list)
{
	if (!list)
		return;
	free(list->items);
	free(list);
}

void garment_list_push(garment_list_t *list, garment_t garment)
{
	assert(list);
	if (list->capacity < list->len + 1) {
		size_t new_capacity = (list->capacity + 1) * 2;

		list->items = (garment_t *)realloc(list->items, new_capacity);
	}

	list->items[list->len++] = garment;
}

void garment_list_remove_index(garment_list_t *list, size_t index)
{
	assert(list);
	assert(index < list->len);
	memmove(&list->items[index], &list->items[index + 1],
		(list->len - index - 1) * sizeof(garment_t));
	list->len--;
}
