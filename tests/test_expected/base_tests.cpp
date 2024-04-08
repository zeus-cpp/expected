#include <string>

#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

TEST_CASE("default constructable to T()", "[default-constructable-to-T]")
{
    expected<int, int> e1;
    expected<int, int> e2 {};
    REQUIRE(e1);
    REQUIRE(e2);
    REQUIRE(e1.value() == 0);
    REQUIRE(e2.value() == 0);
}

TEST_CASE("not default-constructable if T is not default-constructable", "not-default-constructable-if-T-is-not-default-constructable")
{
    struct NotDefaultConstructable
    {
        NotDefaultConstructable() = delete;
    };
    // should not compiled
    //expected<NotDefaultConstructable, int> e;
}

TEST_CASE("construct E in place", "[construct-E-in-place]")
{
    expected<void, int> e1 {unexpect};
    expected<void, int> e2 {unexpect, 42};
    expected<int, int>  e3 {unexpect};
    expected<int, int>  e4 {unexpect, 42};
}

TEST_CASE("implicit conversion from T", "[implicit-conversion-from, T]")
{
    expected<int, int> e = 42;
    REQUIRE(e);
    REQUIRE(e.value() == 42);
}

TEST_CASE("explicit conversion from E", "[explicit-conversion-from, E]")
{
    expected<std::string, int> e1 = zeus::unexpected<int>(42);
    expected<std::string, int> e2 {unexpect, 42};
    REQUIRE(!e1);
    REQUIRE(!e2);
    REQUIRE(e1.error() == 42);
    REQUIRE(e2.error() == 42);
}

TEST_CASE("implicit conversion from E", "[implicit-conversion-from, E]")
{
    REQUIRE_FALSE(std::is_convertible_v<expected<std::string, int>, int>);
}

TEST_CASE("expected<T, void>", "[expected-T-void, E]")
{
    // should not compiled
    //expected<int, void> e = 42;
}

TEST_CASE("expected<void, E>", "[expected-void-E, T]")
{
    expected<void, int> e1 = {};
    expected<void, int> e2 = zeus::unexpected(42);
}

TEST_CASE("expected<void, E>::value()", "[expected-void-E, T]")
{
    expected<void, int> e;
    REQUIRE(e.has_value());
    REQUIRE(std::is_same_v<decltype(e.value()), void>);
}

TEST_CASE("emplace(...)", "[emplace, T]")
{
    struct S
    {
        S(int i, double d) noexcept
            : i(i)
            , d(d)
        {
        }
        int    i;
        double d;
    };
    expected<S, std::nullopt_t> e {unexpect, std::nullopt};
    e.emplace(42, 3.14);
    REQUIRE(e);
    REQUIRE(e.value().i == 42);
    REQUIRE(e.value().d == 3.14);
}

TEST_CASE("never-empty", "[never-empty]")
{
    class ThrowOnDefaultConstruct
    {
    public:
        ThrowOnDefaultConstruct() noexcept(false) { throw std::runtime_error("ThrowOnDefaultConstruct"); }
    };
    expected<ThrowOnDefaultConstruct, std::nullopt_t> e1 {unexpect, std::nullopt};
    REQUIRE(true);
    // Okay, should not compiled
    //e1.emplace();
}

TEST_CASE("equality", "[equality]")
{
    expected<int, int> const v1;
    expected<int, int> const v2 {42};
    expected<int, int> const v3 = 42;
    expected<int, int> const e1 {unexpect, 0};
    expected<int, int> const e2 {unexpect, 42};
    expected<int, int> const e3 = zeus::unexpected(42);
    REQUIRE(v1 != v2);
    REQUIRE(v2 == v3);
    REQUIRE(v1 != e1);
    REQUIRE(v1 != e2);
    REQUIRE(e1 != e2);
    REQUIRE(e2 == e3);
    REQUIRE(e1 != v1);
    REQUIRE(e1 != v2);
}

TEST_CASE("unexpected<E>::error()", "[unexpected-error]")
{
    auto e = zeus::unexpected(42);
    REQUIRE(e.error() == 42);
}

TEST_CASE("bad_expected_access<E>::error()", "[bad_expected_access-error]")
{
    expected<void, int> e = zeus::unexpected(42);
    try
    {
        e.value();
        REQUIRE(false);
    }
    catch (bad_expected_access<int> const& ex)
    {
        REQUIRE(ex.error() == 42);
    }
}

TEST_CASE("expected<void, E>::emplace() and construct in-place", "[expected-void-E-emplace]")
{
    expected<void, int> e1 {std::in_place};
    expected<void, int> e2;
    e2.emplace();
}

TEST_CASE("swap", "[swap]")
{
    expected<int, int> e1 {42};
    expected<int, int> e2 {1'337};

    e1.swap(e2);
    CHECK(e1.value() == 1'337);
    CHECK(e2.value() == 42);

    swap(e1, e2);
    CHECK(e1.value() == 42);
    CHECK(e2.value() == 1'337);
}

TEST_CASE("brace constructed value", "[brace_constructed_value]")
{
#if !defined(__GNUC__) // GCC bug
    expected<std::string, int> e {{}};
    CHECK(e);
#endif
}

TEST_CASE("LWG-3836", "[LWG-3836]")
{
    struct BaseError
    {
    };
    struct DerivedError : BaseError
    {
    };

    expected<bool, DerivedError> e1(false);
    expected<bool, BaseError>    e2(e1);
    CHECK(!e2.value());

    expected<void, DerivedError> e3 {};
    expected<void, BaseError>    e4(e3);
    CHECK(e4.has_value());
}

TEST_CASE("assignment", "[assignment]")
{
    // non-void
    {
        // error = value
        {
            expected<int, int> e1 {unexpect, 1'337};
            expected<int, int> e2 {42};

            e1 = e2;
            CHECK(e1);
        }
    }

    // void
    {
        // error = value
        {
            expected<void, int> e1 {unexpect, 1'337};
            expected<void, int> e2 {};

            e1 = e2;
            CHECK(e1);
        }
    }
}

TEST_CASE("github issue #97 and #145", "[tl-github-issues]")
{
    { // #97
        struct user_provided_move
        {
            user_provided_move() = default;
            user_provided_move(user_provided_move&&) noexcept {}
        };

        struct defaulted_move
        {
            defaulted_move()                 = default;
            defaulted_move(defaulted_move&&) = default;
        };

        {
            using Expected = expected<user_provided_move, int>;
            Expected t1;
            Expected tm1(std::move(t1));
            CHECK(tm1);
        }
        {
            using Expected = expected<defaulted_move, int>;
            Expected t2;
            Expected tm2(std::move(t2)); // should compile
            CHECK(tm2);
        }
    }

    { // #145
        class MoveOnly
        {
        public:
            MoveOnly() = default;

            // Non-copyable
            MoveOnly(const MoveOnly&)            = delete;
            MoveOnly& operator=(const MoveOnly&) = delete;

            // Movable trivially
            MoveOnly(MoveOnly&&)            = default;
            MoveOnly& operator=(MoveOnly&&) = default;
        };

        {
            using Expected = expected<MoveOnly, int>;
            Expected a {};
            Expected b = std::move(a); // should compile
        }
    }
}
