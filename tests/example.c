#include "utest.h"

UTEST(example, equal)
{
	EXPECT_EQ(1, 1);
}

UTEST(example, greater_than)
{
	EXPECT_GT(2, 1);
}
