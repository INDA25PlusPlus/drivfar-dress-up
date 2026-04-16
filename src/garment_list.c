#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "garment.h"
#include "garment_list.h"

GarmentList *garmentListCreate()
{
    return (GarmentList *)calloc(1, sizeof(GarmentList));
}

void garmentListDestroy(GarmentList *list)
{
    if (!list)
        return;
    free(list->items);
    free(list);
}

void garmentListPush(GarmentList *list, Garment garment)
{
    assert(list);
    if (list->capacity < list->len + 1) {
        size_t newCapacity = (list->capacity + 1) * 2;

        list->items = (Garment *)realloc(list->items, newCapacity);
    }

    list->items[list->len++] = garment;
}

void garmentListRemoveIndex(GarmentList *list, size_t index)
{
    assert(list);
    assert(index < list->len);
    memmove(&list->items[index], &list->items[index + 1],
            (list->len - index - 1) * sizeof(Garment));
    list->len--;
}
