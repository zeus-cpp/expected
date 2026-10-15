#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

#include <string>
#include <type_traits>
#include <utility>

using namespace zeus;

namespace
{

struct ThrowingMoveError
{
    int value = 0;

    ThrowingMoveError() = default;
    explicit ThrowingMoveError(int v)
        : value(v)
    {
    }
    ThrowingMoveError(ThrowingMoveError const &) = default;
    ThrowingMoveError(ThrowingMoveError &&other) noexcept(false)
        : value(other.value)
    {
    }
    ThrowingMoveError &operator=(ThrowingMoveError const &) = default;
    ThrowingMoveError &operator=(ThrowingMoveError &&other) noexcept(false)
    {
        value = other.value;
        return *this;
    }

    friend void swap(ThrowingMoveError &, ThrowingMoveError &) noexcept {}
};

static_assert(!std::is_nothrow_move_constructible_v<ThrowingMoveError>);

} // namespace

SCENARIO("expected<cv T, E> stores remove_cv_t<T>", "[LWG-3891]")
{
    GIVEN("a cv-qualified scalar value type")
    {
        static_assert(std::is_swappable_v<volatile int>);
        static_assert(std::is_copy_assignable_v<volatile int>);

        WHEN("constructing and reading the value")
        {
            expected<volatile int, int> value {1};

            CHECK(value.value() == 1);
            CHECK(*value == 1);
        }

        WHEN("assigning between expected objects")
        {
            expected<volatile int, int> lhs {1};
            expected<volatile int, int> rhs {2};

            lhs = rhs;
            CHECK(lhs.value() == 2);

            lhs = std::move(rhs);
            CHECK(lhs.value() == 2);
        }

        WHEN("assigning a value")
        {
            expected<volatile int, int> e {zeus::unexpect, 0};

            e = 3;
            REQUIRE(e.has_value());
            CHECK(e.value() == 3);
        }

        WHEN("transitioning between the value and the error state")
        {
            expected<volatile int, int> e {1};

            e = zeus::unexpected<int> {4};
            REQUIRE_FALSE(e.has_value());
            CHECK(e.error() == 4);

            e = 5;
            REQUIRE(e.has_value());
            CHECK(e.value() == 5);
        }

        WHEN("swapping")
        {
            expected<volatile int, int> lhs {1};
            expected<volatile int, int> rhs {2};

            lhs.swap(rhs);
            CHECK(lhs.value() == 2);
            CHECK(rhs.value() == 1);

            expected<volatile int, int> err {zeus::unexpect, 9};
            lhs.swap(err);
            REQUIRE_FALSE(lhs.has_value());
            CHECK(lhs.error() == 9);
            REQUIRE(err.has_value());
            CHECK(err.value() == 2);
        }

        WHEN("emplacing")
        {
            expected<volatile int, int> e {1};

            CHECK(e.emplace(7) == 7);
            CHECK(e.value() == 7);
        }
    }

    GIVEN("a cv-qualified value type with a non-trivial error type")
    {
        WHEN("assigning and swapping")
        {
            expected<volatile int, std::string> v {1};
            expected<volatile int, std::string> e {zeus::unexpect, "boom"};

            v = e;
            REQUIRE_FALSE(v.has_value());
            CHECK(v.error() == "boom");

            v = 2;
            REQUIRE(v.has_value());
            CHECK(v.value() == 2);

            v.swap(e);
            REQUIRE_FALSE(v.has_value());
            CHECK(v.error() == "boom");
            REQUIRE(e.has_value());
            CHECK(e.value() == 2);
        }
    }

    GIVEN("a cv-qualified value type whose error type is not nothrow move constructible")
    {
        WHEN("swapping mixed states")
        {
            expected<volatile int, ThrowingMoveError> v {1};
            expected<volatile int, ThrowingMoveError> e {zeus::unexpect, ThrowingMoveError {9}};

            v.swap(e);
            REQUIRE_FALSE(v.has_value());
            CHECK(v.error().value == 9);
            REQUIRE(e.has_value());
            CHECK(e.value() == 1);
        }
    }

    GIVEN("a const-qualified value type")
    {
        static_assert(!std::is_copy_assignable_v<expected<const int, int>>);
        static_assert(!std::is_move_assignable_v<expected<const int, int>>);
        static_assert(!std::is_swappable_v<expected<const int, int>>);

        WHEN("constructing, reading and emplacing")
        {
            expected<const int, int> e {1};

            const int &r = e.value();
            CHECK(r == 1);

            CHECK(e.emplace(2) == 2);
            CHECK(e.value() == 2);
        }

        WHEN("accessing through a cv-qualified glvalue")
        {
            expected<const volatile int, int> e {3};

            const volatile int &r = e.value();
            CHECK(r == 3);
            CHECK(*e == 3);
            CHECK(e.operator->() != nullptr);
        }
    }
}
