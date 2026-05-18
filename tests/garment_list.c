#include "garment_list.h"
#include "utest.h"

UTEST(garmentList, addElements)
{
    GarmentList *list = garmentListCreate();
    ASSERT_NE(list, NULL);

    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        garmentListPush(list,
                        (Garment){ .id = id, .color = COLOR_CERISE_STRONG });
    }

    for (GarmentId id = 0; id < GARMENT_COUNT; id++) {
        EXPECT_EQ(list->items[id].id, id);
    }

    garmentListDestroy(list);
}
