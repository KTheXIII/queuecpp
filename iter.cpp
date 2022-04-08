#include <iostream>
#include <cstdint>

#include "queue.hpp"

auto main([[maybe_unused]]std::int32_t argc, [[maybe_unused]]char const* argv[]) -> std::int32_t {
    std::cout << "C++ Iterator Implementation" << "\n";
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    constexpr auto SIZE = 4;
    nerv::queue<std::uint32_t, SIZE> test{};
    test.enq(1);
    test.enq(2);
    test.enq(3);
    test.enq(4);

    std::for_each(std::begin(test), std::end(test), [i = 0](auto const& v) mutable {
        std::cout << v;
        if (i++ < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    });
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    std::for_each(test.crbegin(), test.crend(), [i = 0](auto v) mutable {
        std::cout << v;
        if (i++ < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    });
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    std::cout << "max: " << *std::max_element(std::begin(test), std::end(test)) << "\n";;
    auto it = std::find(std::begin(test), std::end(test), 2);
    std::cout << *it << "\n";

    //std::for_each(std::begin(test), std::end(test), [i = 0](auto const& v) mutable {
    //    std::cout << v;
    //    if (i++ < SIZE - 1) std::cout << " · ";
    //    else std::cout << "\n";
    //});
    //std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";
    auto it2 = std::rbegin(test);
    for (auto i = 0; i < SIZE; i++) {
        auto value = it2++;
        std::cout << *value;
        if (i < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    }
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";
    auto it3 = std::rbegin(test);
    for (auto i = 0; i < SIZE; i++) {
        auto value = it3;
        std::cout << *value;
        it3 -= 2;
        if (it3 == std::rend(test)) {
            std::cout << "\n";
            break;
        }
        if (i < SIZE - 1) std::cout << " · ";
        else std::cout << "\n";
    }
    std::cout << "┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄" << "\n";

    return 0;
}
