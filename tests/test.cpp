#include <gtest/gtest.h>

int Add(int a, int b)
{
	return a + b;
}

TEST(test, test)
{
	EXPECT_EQ(Add(1, 2), 3);
	ASSERT_EQ(Add(2, 3), 5);
}
