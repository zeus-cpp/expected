#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

#include <utility>

namespace
{

// Comparison results need only an implicit conversion to bool, not explicit conversion or negation.
struct ImplicitBoolean
{
    bool value;

    constexpr operator bool() const noexcept { return value; }
    explicit  operator bool()   = delete;
    bool      operator!() const = delete;
};

template<class T>
struct EqualityOnlyError
{
    T value;
};

template<class T, class U>
constexpr ImplicitBoolean operator==(const EqualityOnlyError<T> &lhs, const EqualityOnlyError<U> &rhs) noexcept
{
    return {lhs.value == rhs.value};
}

// Like Boost.Interval's "certain" comparisons, equality and inequality can both be false.
struct IntervalError
{
    int lower;
    int upper;

    friend constexpr bool operator==(const IntervalError &lhs, const IntervalError &rhs) noexcept
    {
        return lhs.lower == lhs.upper && rhs.lower == rhs.upper && lhs.lower == rhs.lower;
    }

    friend constexpr bool operator!=(const IntervalError &lhs, const IntervalError &rhs) noexcept(false)
    {
        return lhs.upper < rhs.lower || rhs.upper < lhs.lower;
    }
};

struct ComparisonFailure
{
};

struct ThrowingBoolean
{
    operator bool() const { throw ComparisonFailure {}; }
};

struct ConversionThrows
{
    friend ThrowingBoolean operator==(const ConversionThrows &, const ConversionThrows &) noexcept { return {}; }
    friend ThrowingBoolean operator!=(const ConversionThrows &, const ConversionThrows &) noexcept { return {}; }
};

struct ComparisonThrows
{
    friend bool operator==(const ComparisonThrows &, const ComparisonThrows &) { throw ComparisonFailure {}; }
    friend bool operator!=(const ComparisonThrows &, const ComparisonThrows &) noexcept { return false; }
};

} // namespace

TEST_CASE("unexpected inequality supports equality-only errors with implicit bool results", "[unexpected][equality]")
{
    constexpr zeus::unexpected<EqualityOnlyError<int>>  lhs {EqualityOnlyError<int> {42}};
    constexpr zeus::unexpected<EqualityOnlyError<int>>  equal {EqualityOnlyError<int> {42}};
    constexpr zeus::unexpected<EqualityOnlyError<int>>  different {EqualityOnlyError<int> {7}};
    constexpr zeus::unexpected<EqualityOnlyError<long>> other_equal {EqualityOnlyError<long> {42}};
    constexpr zeus::unexpected<EqualityOnlyError<long>> other_different {EqualityOnlyError<long> {7}};

    STATIC_REQUIRE(noexcept(lhs != equal));
    STATIC_REQUIRE(noexcept(lhs != other_equal));
    STATIC_REQUIRE(noexcept(other_equal != lhs));

    STATIC_REQUIRE(lhs == equal);
    STATIC_REQUIRE_FALSE(lhs != equal);
    STATIC_REQUIRE(lhs != different);
    STATIC_REQUIRE_FALSE(lhs != other_equal);
    STATIC_REQUIRE_FALSE(other_equal != lhs);
    STATIC_REQUIRE(lhs != other_different);
    STATIC_REQUIRE(other_different != lhs);

    CHECK(lhs == equal);
    CHECK_FALSE(lhs != equal);
    CHECK(lhs != different);
    CHECK_FALSE(lhs != other_equal);
    CHECK_FALSE(other_equal != lhs);
    CHECK(lhs != other_different);
    CHECK(other_different != lhs);
}

TEST_CASE("unexpected inequality negates equality independently of error inequality", "[unexpected][equality][noexcept]")
{
    constexpr zeus::unexpected<IntervalError> lhs {
        IntervalError {1, 3}
    };
    constexpr zeus::unexpected<IntervalError> rhs {
        IntervalError {2, 4}
    };

    STATIC_REQUIRE(noexcept(lhs != rhs));

    STATIC_REQUIRE_FALSE(lhs.error() == rhs.error());
    STATIC_REQUIRE_FALSE(lhs.error() != rhs.error());
    STATIC_REQUIRE(lhs != rhs);
    STATIC_REQUIRE(rhs != lhs);

    CHECK(lhs != rhs);
    CHECK(rhs != lhs);
}

TEST_CASE("unexpected comparisons propagate exceptions from bool conversion", "[unexpected][equality][noexcept]")
{
    const zeus::unexpected<ConversionThrows> lhs {std::in_place};
    const zeus::unexpected<ConversionThrows> rhs {std::in_place};

    STATIC_REQUIRE_FALSE(noexcept(lhs == rhs));
    STATIC_REQUIRE_FALSE(noexcept(lhs != rhs));
    CHECK_THROWS_AS(lhs == rhs, ComparisonFailure);
    CHECK_THROWS_AS(lhs != rhs, ComparisonFailure);
}

TEST_CASE("unexpected inequality propagates exceptions from error equality", "[unexpected][equality][noexcept]")
{
    const zeus::unexpected<ComparisonThrows> lhs {std::in_place};
    const zeus::unexpected<ComparisonThrows> rhs {std::in_place};

    STATIC_REQUIRE_FALSE(noexcept(lhs != rhs));
    CHECK_THROWS_AS(lhs != rhs, ComparisonFailure);
}
