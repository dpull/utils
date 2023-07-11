#include "gtest/gtest.h"
#include "circular_buffer.h"

TEST(circular_buffer, push)
{
    const int size = 4;
    auto circular_buffer = circular_buffer_create(size);
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_pushback(circular_buffer);
        value->value = i;
    }
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_popfront(circular_buffer);
        ASSERT_EQ(value->value, i);
    }
    circular_buffer_destory(circular_buffer);
}

TEST(circular_buffer, push_more)
{
    const int size = 4;
    auto circular_buffer = circular_buffer_create(size);
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_pushback(circular_buffer);
        value->value = i;
    }
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_pushback(circular_buffer);
        ASSERT_EQ(value, nullptr);
    }
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_popfront(circular_buffer);
        ASSERT_EQ(value->value, i);
    }
    circular_buffer_destory(circular_buffer);
}

TEST(circular_buffer, pop)
{
    const int size = 4;
    auto circular_buffer = circular_buffer_create(size);
    for (auto i = 0; i < size; ++i) {
        auto value = circular_buffer_pushback(circular_buffer);
        value->value = i;
    }
    for (auto i = 0; i < size / 2; ++i) {
        auto value = circular_buffer_popfront(circular_buffer);
        ASSERT_EQ(value->value, i);
    }
    for (auto i = 0; i < size / 2; ++i) {
        auto value = circular_buffer_pushback(circular_buffer);
        value->value = i;
    }
    circular_buffer_destory(circular_buffer);
}
