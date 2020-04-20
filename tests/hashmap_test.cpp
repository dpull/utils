#include "gtest/gtest.h"
#include "hashmap.h"

TEST(hashmap, add)
{
    const int size = 4;
    auto hashmap = hashmap_create(size);
    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_add(hashmap, i);
        value->value = i;
    }

    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_get(hashmap, i);
        ASSERT_EQ(value->value, i);
    }

    hashmap_destory(hashmap);
}

TEST(hashmap, remove)
{
    const int size = 4;
    auto hashmap = hashmap_create(size);
    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_add(hashmap, i);
        value->value = i;
    }

    for (auto i = 1; i <= size; ++i) {
        if (i % 2 == 0) {
            hashmap_remove(hashmap, i);
        }
    }

    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_get(hashmap, i);
        if (i % 2 == 0) {
            ASSERT_EQ(value, nullptr);
            continue;
        }
        ASSERT_EQ(value->value, i);
    }

    hashmap_destory(hashmap);
}

TEST(hashmap, add_more)
{
    const int size = 4;
    auto hashmap = hashmap_create(size);
    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_add(hashmap, i);
        value->value = i;
    }

    for (auto i = 1; i <= size; ++i) {
        auto value = hashmap_add(hashmap, i);
        ASSERT_EQ(value, nullptr);
    }

    for (auto i = size + 1; i <= size + 2; ++i) {
        auto value = hashmap_add(hashmap, i);
        ASSERT_EQ(value, nullptr);
    }

    for (auto i = size + 1; i <= size + 2; ++i) {
        auto value = hashmap_get(hashmap, i);
        ASSERT_EQ(value, nullptr);
    }

    hashmap_destory(hashmap);
}
