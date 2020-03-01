#include <stdio.h>
#include "gtest/gtest.h"

int main(int argc, char **argv)
{
#if GTEST_LINKED_AS_SHARED_LIBRARY
	printf("GTEST_LINKED_AS_SHARED_LIBRARY is on\n");
#else
	printf("GTEST_LINKED_AS_SHARED_LIBRARY is off\n");
#endif
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
