#include <cstdint>
#include <iostream>
#include <numeric>

#include "gtest/gtest.h"
#include "ring.hpp"

TEST(HeadTail, Empty) {
    nrv::ring<float, 4> ring{};

    auto head_ptr = ring.index_front(0);
    auto tail_ptr = ring.index_back(0);

    ASSERT_EQ(head_ptr, 0);
    ASSERT_EQ(tail_ptr, 0);
}

TEST(HeadTail, Full) {
    nrv::ring<float, 2> ring{};
    ring.enq(1);
    ring.enq(2);

    auto front = ring.index_front(0);
    auto back  = ring.index_back(0);

    ASSERT_EQ(front, 0);
    ASSERT_EQ(back, 2);
}

TEST(BeginEnd, Empty) {
    nrv::ring<float, 2> ring{};
    auto begin = ring.begin();
    auto end = ring.end();
    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 1);
}
TEST(BeginEnd, Partial) {
    nrv::ring<float, 2> ring{};
    ring.enq(1);
    auto begin = ring.begin();
    auto end = ring.end();

    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 2);
}
TEST(BeginEnd, Full) {
    nrv::ring<float, 2> ring{};
    ring.enq(1);
    ring.enq(2);

    auto begin = ring.begin();
    auto end = ring.end();
    ASSERT_EQ(begin.ptr(), 0);
    ASSERT_EQ(end.ptr(), 1);
}

TEST(Is, Empty) {
    nrv::ring<float, 4> ring{};
    ASSERT_TRUE(ring.size() == 0);
}

TEST(Is, Full) {
    nrv::ring<float, 4> ring{};
    ring.enq(1);
    ring.enq(1);
    ring.enq(1);
    ring.enq(2);
    ring.enq_keep(3);

    ASSERT_TRUE(ring.size() == 4);
    std::cout << *ring.rbegin() << "\n";
}

TEST(Sum, Empty) {
    nrv::ring<float, 8> ring{};
    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 0.0f);
}

TEST(Sum, One) {
    nrv::ring<float, 8> ring{};
    ring.enq(1.0f);
    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 1.0f);
}

TEST(Sum, Half) {
    nrv::ring<float, 8> ring{};
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 4.0f);
}

TEST(Sum, Full) {
    nrv::ring<float, 8> ring{};
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);

    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 8.0f);
}

TEST(Sum, FullKeep) {
    nrv::ring<float, 8> ring{};
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);

    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);

    ring.enq_keep(3.0f);
    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 8.0f);
}

TEST(Sum, Override) {
    nrv::ring<float, 8> ring{};
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);

    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);
    ring.enq(1.0f);

    ring.enq(2.0f);

    auto sum = std::accumulate(std::begin(ring), std::end(ring), 0.0f, std::plus<>());
    ASSERT_EQ(sum, 9.0f);
}

auto main(std::int32_t argc, char const* argv[]) -> std::int32_t {
    testing::InitGoogleTest(&argc, (char**)argv);
    return RUN_ALL_TESTS();
}

