#include "garment.h"
#include "ui/state.h"
#include "utest.h"


// We declare these functions outside a dedicated header file since they are
// only used in their original C file and in tests.
bool getNextGarment(GarmentId start, GarmentType type, GarmentId *result);
bool getPreviousGarment(GarmentId start, GarmentType type, GarmentId *result);

UTEST(interaction, getNextGarment)
{
    GarmentId result;
    
    EXPECT_TRUE(getNextGarment(GARMENT_TEST_A, GARMENT_TYPE_TOP, &result));
	EXPECT_EQ(result, GARMENT_TEST_A);
    
    EXPECT_FALSE(getNextGarment(GARMENT_TEST_A + 1, GARMENT_TYPE_TOP, &result));
    
    EXPECT_TRUE(getNextGarment(GARMENT_PANTS_A + 1, GARMENT_TYPE_BOTTOM, &result));
	EXPECT_EQ(result, GARMENT_SKIRT_A);
    
    EXPECT_TRUE(getNextGarment(GARMENT_SKIRT_A, GARMENT_TYPE_BOTTOM, &result));
	EXPECT_EQ(result, GARMENT_SKIRT_A);
}

UTEST(interaction, getPreviousGarment)
{
    GarmentId result;
    
    EXPECT_TRUE(getPreviousGarment(GARMENT_TEST_A, GARMENT_TYPE_TOP, &result));
	EXPECT_EQ(result, GARMENT_TEST_A);
    
    EXPECT_TRUE(getPreviousGarment(GARMENT_PANTS_A, GARMENT_TYPE_BOTTOM, &result));
    
    EXPECT_FALSE(getPreviousGarment(GARMENT_PANTS_A - 1, GARMENT_TYPE_BOTTOM, &result));
    
    EXPECT_TRUE(getPreviousGarment(GARMENT_SKIRT_A - 1, GARMENT_TYPE_BOTTOM, &result));
	EXPECT_EQ(result, GARMENT_PANTS_A);
}

