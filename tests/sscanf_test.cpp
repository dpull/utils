#include "gtest/gtest.h"

static void ASSERT_EQ_ARRAY(char* array1, char* array2, int count) {
    for (auto i = 0; i < count; ++i)
    {
        ASSERT_EQ(array1[i], array2[i]);
    }
}

TEST(sscanf, test1)
{
    int i, j;
    int ret = sscanf("5 6", "%d%d", &i, &j);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 6);
}

TEST(sscanf, test2)
{
    int i, j;
    int ret = sscanf("5\t6", "%d%d", &i, &j);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 6);
}

TEST(sscanf, test3)
{
    int i, j;
    int ret = sscanf("5\t   6", "%d%d", &i, &j);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 6);
}

TEST(sscanf, test4)
{
    int i, j;
    int ret = sscanf("5\n6", "%d%d", &i, &j);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 6);
}

TEST(sscanf, test5)
{
    int i, j;
    int ret = sscanf("5\n6s", "%d%d", &i, &j);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 6);
}

TEST(sscanf, test6)
{
    int i, j = 0;
    int ret = sscanf("5 s6", "%d%d", &i, &j);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(i, 5);
    ASSERT_EQ(j, 0);
}

TEST(sscanf, test7)
{
    int i;
    char array1[4];
    char array2[] = { '1', '2', '\0', -1 };

    memset(array1, -1, sizeof(array1));
    int ret = sscanf("5 1234", "%d%2s", &i, array1);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ_ARRAY(array1, array2, sizeof(array1));
}

TEST(sscanf, test8)
{
    int i;
    char array1[4];
    char array2[] = { '1', '\0', -1, -1 };

    memset(array1, -1, sizeof(array1));
    int ret = sscanf("5 1\t\t\t\t", "%d%2s", &i, array1);
    ASSERT_EQ(ret, 2);
    ASSERT_EQ(i, 5);
    ASSERT_EQ_ARRAY(array1, array2, sizeof(array1));
}
