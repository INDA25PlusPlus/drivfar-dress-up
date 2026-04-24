#ifndef GARMENT_LIST_H
#define GARMENT_LIST_H

#include "garment.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

typedef struct {
    size_t len;
    size_t capacity;
    Garment *items;
} GarmentList;

GarmentList *garmentListCreate();

void garmentListDestroy(GarmentList *list);

void garmentListPush(GarmentList *list, Garment garment);

// Removes the specified index from the list, shifting the other elements after
// it one index down.
void garmentListRemoveIndex(GarmentList *list, size_t index);

#endif
