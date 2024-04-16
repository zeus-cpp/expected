#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

struct FromType
{
};

enum class NoThrowConvertible
{
    Yes,
    No,
};

template<NoThrowConvertible N>
struct ToType;

template<>
struct ToType<NoThrowConvertible::Yes>
{
    ToType()              = default;
    ToType(ToType const&) = default;
    ToType(ToType&&)      = default;

    ToType(FromType const&) noexcept {}
};

template<>
struct ToType<NoThrowConvertible::No>
{
    ToType()              = default;
    ToType(ToType const&) = default;
    ToType(ToType&&)      = default;

    ToType(FromType const&) noexcept(false) {}
};

TEST_CASE("value_or() noexcept", "[noexcept, value_or]")
{
    { // noexcept(true)
        using T        = ToType<NoThrowConvertible::Yes>;
        using E        = int;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK(noexcept(e.value_or(FromType {})));
    }
    { // noexcept(false)
        using T        = ToType<NoThrowConvertible::No>;
        using E        = int;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK_FALSE(noexcept(e.value_or(FromType {})));
    }
}

TEST_CASE("error_or() noexcept", "[noexcept, error_or]")
{
    { // noexcept(true)
        using T        = int;
        using E        = ToType<NoThrowConvertible::Yes>;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK(noexcept(e.error_or(FromType {})));
    }
    { // noexcept(false)
        using T        = int;
        using E        = ToType<NoThrowConvertible::No>;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK_FALSE(noexcept(e.error_or(FromType {})));
    }
}

TEST_CASE("void-T error_or() noexcept", "[noexcept, error_or, void-T]")
{
    { // noexcept(true)
        using T        = void;
        using E        = ToType<NoThrowConvertible::Yes>;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK(noexcept(e.error_or(FromType {})));
    }
    { // noexcept(false)
        using T        = void;
        using E        = ToType<NoThrowConvertible::No>;
        using Expected = zeus::expected<T, E>;
        Expected e;
        CHECK_FALSE(noexcept(e.error_or(FromType {})));
    }
}
