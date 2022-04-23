#include <cstdint>
#include <iostream>

#include "gtest/gtest.h"

TEST(TestName, SubTest_1) {
    ASSERT_TRUE(1 == 2);
}

auto main([[maybe_unused]]std::int32_t argc, [[maybe_unused]]char const* argv[]) -> std::int32_t {
    testing::InitGoogleTest(&argc, (char**)argv);
    return RUN_ALL_TESTS();
}

