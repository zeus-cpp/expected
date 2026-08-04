#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

namespace
{

template<class T1, class T2, class = void>
struct is_eq_well_formed : std::false_type
{
};

template<class T1, class T2>
struct is_eq_well_formed<T1, T2, std::void_t<decltype(std::declval<const T1 &>() == std::declval<const T2 &>())>> : std::true_type
{
};

template<class T1, class T2, class = void>
struct is_ne_well_formed : std::false_type
{
};

template<class T1, class T2>
struct is_ne_well_formed<T1, T2, std::void_t<decltype(std::declval<const T1 &>() != std::declval<const T2 &>())>> : std::true_type
{
};

struct NotBool
{
};

struct NotBoolCompareResult
{
};

[[maybe_unused]] NotBoolCompareResult operator==(const NotBool &, const NotBool &)
{
    return {};
}

struct Bool
{
    bool     value;
             operator bool() const { return value; }
    explicit operator bool() = delete;
};

struct E1
{
};

struct E2
{
};

Bool operator==(E1, E2)
{
    return {true};
}

struct ValueWithBool
{
    int  v;
    Bool operator==(const ValueWithBool &rhs) const { return {v == rhs.v}; }
};

struct IntEqResult
{
    int v;
};
[[maybe_unused]] int operator==(const IntEqResult &lhs, const IntEqResult &rhs)
{
    return lhs.v == rhs.v;
}

} // namespace

TEST_CASE("P3379: expected<T,E> == expected<T2,E2> excluded when T,T2 not eq-comparable to bool", "[P3379][equality]")
{
    using Ex1 = expected<int, int>;
    using Ex2 = expected<int, int>;
    STATIC_REQUIRE((is_eq_well_formed<const Ex1 &, const Ex2 &>::value));

    using ExWithNB = expected<NotBool, int>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const ExWithNB &, const Ex1 &>::value));
}

TEST_CASE("P3379: expected<T,E> == expected<T2,E2> excluded when E,E2 not eq-comparable to bool", "[P3379][equality]")
{
    using Ex1 = expected<int, int>;
    STATIC_REQUIRE((is_eq_well_formed<const Ex1 &, const Ex1 &>::value));

    using ExNB = expected<int, NotBool>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const ExNB &, const ExNB &>::value));
}

TEST_CASE("P3379: expected<T,E> == T2 excluded when T,T2 not eq-comparable to bool", "[P3379][equality]")
{
    STATIC_REQUIRE((is_eq_well_formed<const expected<int, int> &, const int &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const expected<NotBool, int> &, const NotBool &>::value));
}

TEST_CASE("P3379: expected<T,E> == unexpected<E2> excluded when E,E2 not comparable", "[P3379][equality]")
{
    STATIC_REQUIRE((is_eq_well_formed<const expected<int, int> &, const zeus::unexpected<int> &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const expected<int, NotBool> &, const zeus::unexpected<NotBool> &>::value));
}

TEST_CASE("P3379: expected<void,E> == expected<T2,E2> excluded when E,E2 not comparable", "[P3379][equality]")
{
    using VEx = expected<void, int>;
    STATIC_REQUIRE((is_eq_well_formed<const VEx &, const VEx &>::value));

    using VExNB = expected<void, NotBool>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const VExNB &, const VEx &>::value));
}

TEST_CASE("P3379: expected<void,E> == unexpected<E2> excluded when E,E2 not comparable", "[P3379][equality]")
{
    STATIC_REQUIRE((is_eq_well_formed<const expected<void, int> &, const zeus::unexpected<int> &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const expected<void, NotBool> &, const zeus::unexpected<NotBool> &>::value));
}

TEST_CASE("P3379: expected<T,E> == expected<void,E2> excluded by !is_void_v<T2>", "[P3379][equality]")
{
    using Ex1    = expected<int, int>;
    using VoidEx = expected<void, int>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const Ex1 &, const VoidEx &>::value));
}

TEST_CASE("P3379: expected<void,E> == expected<int,E2> excluded by is_void_v<T2>", "[P3379][equality]")
{
    using VoidEx = expected<void, int>;
    using Ex1    = expected<int, int>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const VoidEx &, const Ex1 &>::value));
}

TEST_CASE("P3379: expected<T,E> != expected<T2,E2> excluded when comparisons not convertible", "[P3379][equality]")
{
    using Ex1 = expected<int, int>;
    STATIC_REQUIRE((is_ne_well_formed<const Ex1 &, const Ex1 &>::value));

    using ExNB = expected<NotBool, int>;
    STATIC_REQUIRE_FALSE((is_ne_well_formed<const ExNB &, const Ex1 &>::value));
}

TEST_CASE("P3379: expected<T,E> != T2 excluded when T,T2 not convertible to bool", "[P3379][equality]")
{
    STATIC_REQUIRE((is_ne_well_formed<const expected<int, int> &, const int &>::value));
    STATIC_REQUIRE_FALSE((is_ne_well_formed<const expected<NotBool, int> &, const NotBool &>::value));
}

TEST_CASE("P3379: expected<T,E> != unexpected<E2> excluded when E,E2 not convertible", "[P3379][equality]")
{
    STATIC_REQUIRE((is_ne_well_formed<const expected<int, int> &, const zeus::unexpected<int> &>::value));
    STATIC_REQUIRE_FALSE((is_ne_well_formed<const expected<int, NotBool> &, const zeus::unexpected<NotBool> &>::value));
}

TEST_CASE("P3379: T2 == expected<T,E> excluded when T,T2 not convertible to bool", "[P3379][equality]")
{
    STATIC_REQUIRE((is_eq_well_formed<const int &, const expected<int, int> &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const NotBool &, const expected<NotBool, int> &>::value));
}

TEST_CASE("P3379: T2 != expected<T,E> excluded when T,T2 not convertible to bool", "[P3379][equality]")
{
    STATIC_REQUIRE((is_ne_well_formed<const int &, const expected<int, int> &>::value));
    STATIC_REQUIRE_FALSE((is_ne_well_formed<const NotBool &, const expected<NotBool, int> &>::value));
}

TEST_CASE("LWG 4366: expected<T,E> == unexpected<E2> (error path) compiles with deleted explicit op bool", "[LWG-4366][equality]")
{
    zeus::unexpected  e1 {E1 {}};
    zeus::unexpected  e2 {E2 {}};
    expected<int, E1> exp {e1};
    auto              result = exp == e2;
    CHECK(result);
}

TEST_CASE("LWG 4366: expected<T,E> == expected<T2,E2> (error path) compiles with deleted explicit op bool", "[LWG-4366][equality]")
{
    zeus::unexpected  e1 {E1 {}};
    zeus::unexpected  e2 {E2 {}};
    expected<int, E1> exp1 {e1};
    expected<int, E2> exp2 {e2};
    auto              result = exp1 == exp2;
    CHECK(result);
}

TEST_CASE("LWG 4366: expected<T,E> == expected<T2,E2> (value path) compiles with deleted explicit op bool", "[LWG-4366][equality]")
{
    expected<ValueWithBool, int> x {ValueWithBool {42}};
    expected<ValueWithBool, int> y {ValueWithBool {42}};
    auto                         result = x == y;
    CHECK(result);
}

TEST_CASE("LWG 4366: expected<T,E> == T2 (value path) compiles with deleted explicit op bool", "[LWG-4366][equality]")
{
    ValueWithBool                x {42};
    ValueWithBool                y {42};
    expected<ValueWithBool, int> exp {x};
    auto                         result = exp == y;
    CHECK(result);
}

TEST_CASE("LWG 4366: expected<T,E> == T2 (value path) returns false when values differ (Bool result)", "[LWG-4366][equality]")
{
    ValueWithBool                x {42};
    ValueWithBool                y {99};
    expected<ValueWithBool, int> exp {x};
    auto                         result = exp == y;
    CHECK_FALSE(result);
}

TEST_CASE("LWG 4366: expected<void,E> == expected<T2,E2> (|| rewrite) compiles with deleted explicit op bool", "[LWG-4366][equality]")
{
    zeus::unexpected   e1 {E1 {}};
    zeus::unexpected   e2 {E2 {}};
    expected<void, E1> exp1 {e1};
    expected<void, E2> exp2 {e2};
    auto               result = exp1 == exp2;
    CHECK(result);
}

TEST_CASE("P3379: expected<expected<int,int>, E> == expected<expected<int,int>, E> is allowed", "[P3379][equality]")
{
    using Nested = expected<expected<int, int>, int>;
    STATIC_REQUIRE((is_eq_well_formed<const Nested &, const Nested &>::value));
    STATIC_REQUIRE((is_ne_well_formed<const Nested &, const Nested &>::value));
}

TEST_CASE("P3379: expected<int, expected<int,int>> == expected<int, expected<int,int>> is allowed", "[P3379][equality]")
{
    using Nested = expected<int, expected<int, int>>;
    STATIC_REQUIRE((is_eq_well_formed<const Nested &, const Nested &>::value));
}

TEST_CASE("P3379: expected<int, expected<int,int>> == unexpected<expected<int,int>> is allowed", "[P3379][equality]")
{
    STATIC_REQUIRE((is_eq_well_formed<const expected<int, expected<int, int>> &, const zeus::unexpected<expected<int, int>> &>::value));
}

TEST_CASE("P3379: expected<T,E> == expected<T2,E2> excluded when only T2 (rhs value type) not eq-comparable to bool", "[P3379][equality]")
{
    using Ex1 = expected<int, int>;
    using Ex2 = expected<NotBool, int>;
    STATIC_REQUIRE((is_eq_well_formed<const Ex1 &, const Ex1 &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const Ex1 &, const Ex2 &>::value));
}

TEST_CASE("P3379: expected<T,E> == expected<T2,E2> excluded when only E or only E2 not eq-comparable to bool", "[P3379][equality]")
{
    using Ok     = expected<int, int>;
    using BadErr = expected<int, NotBool>;
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const Ok &, const BadErr &>::value));
    STATIC_REQUIRE_FALSE((is_eq_well_formed<const BadErr &, const Ok &>::value));
}

TEST_CASE(
    "P3379: expected<T,E> == expected<expected<int,int>, E2> routes through operator==(expected, expected<T2,E2>) (T2 is an expected "
    "specialization)",
    "[P3379][equality]"
)
{
    using E1    = expected<int, int>;
    using Cross = expected<expected<int, int>, int>;
    STATIC_REQUIRE((is_eq_well_formed<const E1 &, const Cross &>::value));
    STATIC_REQUIRE((is_ne_well_formed<const E1 &, const Cross &>::value));
    STATIC_REQUIRE((is_eq_well_formed<const Cross &, const E1 &>::value));
}

TEST_CASE("P3379: comparison result implicitly convertible to bool (non-bool result type) satisfies constraints", "[P3379][equality]")
{
    using Ex = expected<IntEqResult, int>;
    STATIC_REQUIRE((is_eq_well_formed<const Ex &, const Ex &>::value));
    STATIC_REQUIRE((is_ne_well_formed<const Ex &, const Ex &>::value));
    STATIC_REQUIRE((is_eq_well_formed<const Ex &, const IntEqResult &>::value));
}
