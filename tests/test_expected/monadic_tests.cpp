#include <string>

#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

TEST_CASE("and_then()", "[monadic]")
{
    using T        = std::string;
    using Expected = expected<int, T>;

    { // non-void
        using Expected2 = expected<double, T>;

        Expected e = 42;

        auto const newVal = e.and_then([](int x) { return Expected2 {x * 2}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
    { // void
        using Expected2 = expected<void, T>;

        Expected e = 42;

        auto const newVal = e.and_then([](int) -> Expected2 { return Expected2 {}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
}
