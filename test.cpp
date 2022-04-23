#include <cstdint>
#include <iostream>

#include "gtest/gtest.h"
#include "queue.hpp"

TEST(HeadTail, Empty) {
    nerv::queue<float, 4> queue{};

    auto head_ptr = queue.index_front(0);
    auto tail_ptr = queue.index_back(0);

    ASSERT_EQ(head_ptr, 0);
    ASSERT_EQ(tail_ptr, 0);
}

TEST(HeadTail, Full) {
    nerv::queue<float, 2> queue{};
    queue.enq(1);
    queue.enq(2);

    auto front = queue.index_front(0);
    auto back  = queue.index_back(0);

    ASSERT_EQ(front, 0);
    ASSERT_EQ(back, 2);
}

TEST(BeginEnd, Empty) {
    nerv::queue<float, 2> queue{};
    auto begin = queue.begin();
    auto end = queue.end();
    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 1);
}
TEST(BeginEnd, Partial) {
    nerv::queue<float, 2> queue{};
    queue.enq(1);
    auto begin = queue.begin();
    auto end = queue.end();

    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 2);
}
TEST(BeginEnd, Full) {
    nerv::queue<float, 2> queue{};
    queue.enq(1);
    queue.enq(2);

    auto begin = queue.begin();
    auto end = queue.end();
    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 1);
}

auto main(std::int32_t argc, char const* argv[]) -> std::int32_t {
    testing::InitGoogleTest(&argc, (char**)argv);
    return RUN_ALL_TESTS();
}

