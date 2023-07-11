#include "array.h"
#include "gtest/gtest.h"

int int_cmp(int i1, int i2) { return i1 - i2; }
int int_str_cmp(int i1, const char* i2) { return i1 - atoi(i2); }

TEST(array, binary_find)
{
    std::vector<int> a;
    for (int i = 0; i < 10; i += 2) {
        a.push_back(i);
    }

    auto ret = binary_find(a.begin(), a.end(), 2, int_cmp);
    ASSERT_TRUE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 1);

    ret = binary_find(a.begin(), a.end(), 3, int_cmp);
    ASSERT_FALSE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 2);
}


TEST(array, binary_find2)
{
    std::vector<int> a;
    for (int i = 0; i < 10; i += 2) {
        a.push_back(i);
    }

    auto ret = binary_find(a.begin(), a.end(), "2", int_str_cmp);
    ASSERT_TRUE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 1);

    ret = binary_find(a.begin(), a.end(), "3", int_str_cmp);
    ASSERT_FALSE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 2);
}

TEST(array, add_unique)
{
    std::vector<int> a;
    for (int i = 0; i < 10; i += 2) {
        a.push_back(i);
    }

    auto ret = add_unique(a, 2, int_cmp);
    ASSERT_FALSE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 1);

    ret = add_unique(a, 3, int_cmp);
    ASSERT_TRUE(ret.second);
    ASSERT_EQ((ret.first - a.begin()), 2);
}
