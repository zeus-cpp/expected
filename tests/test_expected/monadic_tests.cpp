#include <string>
#include <type_traits>

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

TEST_CASE("void-T and_then()", "[monadic, void-T]")
{
    using T        = std::string;
    using Expected = expected<void, T>;

    { // non-void
        using Expected2 = expected<double, T>;

        Expected e {};

        auto const newVal = e.and_then([]() { return Expected2 {}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
    { // void
        using Expected2 = expected<void, T>;

        Expected e {};

        auto const newVal = e.and_then([]() -> Expected2 { return Expected2 {}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
}

TEST_CASE("or_else()", "[monadic]")
{
    using T        = std::string;
    using Expected = expected<T, int>;

    { // non-void
        using Expected2 = expected<T, double>;

        Expected e {};

        auto const newVal = e.or_else([](auto&&) { return Expected2 {}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
}

TEST_CASE("void-T or_else()", "[monadic, void-T]")
{
    using Expected = expected<void, int>;

    { // non-void
        using Expected2 = expected<void, double>;

        Expected e {};

        auto const newVal = e.or_else([](auto&&) { return Expected2 {}; });

        static_assert(std::is_same_v<std::remove_cv_t<decltype(newVal)>, Expected2>);
    }
}
