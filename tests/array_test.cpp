#include "gtest/gtest.h"
#include "array.h"

int int_cmp(const void* a, const void* b)
{
    int va = *((int*)a);
    int vb = *((int*)b);
	return va - vb;
}

TEST(binary_search, exist)
{
	int array[] = { 11, 22, 33, 44, 55 };
	int count = sizeof(array) / sizeof(array[0]);
	for (int i = 0; i < count; ++i){
		int key = array[i];
        int index = binary_search(&key, array, count, sizeof(array[0]), int_cmp);
        ASSERT_EQ(index, i);
	}
}

TEST(binary_search, not_exist_small)
{
    int array[] = { 11, 22, 33, 44, 55 };
    int count = sizeof(array) / sizeof(array[0]);
    for (int i = 0; i < count; ++i) {
        int key = array[i] - 1;
        int index = binary_search(&key, array, count, sizeof(array[0]), int_cmp);
        ASSERT_EQ(index, ~i);
    }
}

TEST(binary_search, not_exist_large)
{
    int array[] = { 11, 22, 33, 44, 55 };
    int count = sizeof(array) / sizeof(array[0]);
    for (int i = 0; i < count; ++i) {
        int key = array[i] + 1;
        int index = binary_search(&key, array, count, sizeof(array[0]), int_cmp);
        ASSERT_EQ(index, ~(i + 1));
    }
}