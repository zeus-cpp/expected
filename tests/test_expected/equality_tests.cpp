#include <string>

#include <catch2/catch_all.hpp>

#include <catch2/catch_test_macros.hpp>
#include <zeus/expected.hpp>

using namespace zeus;

namespace
{

template<bool ShouldEqual, typename T, typename U>
constexpr bool EqualityTester(T const& lhs, U const& rhs)
{
    return (lhs == rhs) == ShouldEqual && (lhs != rhs) != ShouldEqual && (rhs == lhs) == ShouldEqual && (rhs != lhs) != ShouldEqual;
}

struct Type1
{
    std::string value;

    Type1(std::string const& v)
        : value(v)
    {
    }
    Type1(const char* v)
        : value(v)
    {
    }
    Type1(Type1 const&)            = delete;
    Type1& operator=(Type1 const&) = delete;
    Type1(Type1&& other)           = default;
    Type1& operator=(Type1&&)      = default;
    ~Type1()                       = default;

    bool operator==(Type1 const& rhs) const { return value == rhs.value; }
};

struct Type2
{
    std::string value;

    Type2(std::string const& v)
        : value(v)
    {
    }
    Type2(const char* v)
        : value(v)
    {
    }
    Type2(Type2 const&)            = delete;
    Type2& operator=(Type2 const&) = delete;
    Type2(Type2&& other)           = default;
    Type2& operator=(Type2&&)      = default;
    ~Type2()                       = default;

    bool operator==(Type2 const& rhs) const { return value == rhs.value; }
};

inline bool operator==(Type1 const& lhs, Type2 const& rhs)
{
    return lhs.value == rhs.value;
}

inline bool operator==(Type2 const& lhs, Type1 const& rhs)
{
    return rhs == lhs;
}

} // namespace

SCENARIO("equality", "[equality]")
{
    GIVEN("expected<T, E> (T is not void)")
    {
        using T        = Type1;
        using E        = Type2;
        using Expected = expected<T, E>;

        WHEN("compare with same type expected<T, E>")
        {
            Expected const value1     = "value1";
            Expected const value2     = "value2";
            Expected const value1Copy = "value1";
            Expected const error1     = zeus::unexpected<E>("error1");
            Expected const error2     = zeus::unexpected<E>("error2");
            Expected const error1Copy = zeus::unexpected<E>("error1");

            WHEN("two same values")
            {
                CHECK(EqualityTester<true>(value1, value1Copy));
            }
            WHEN("two different values")
            {
                CHECK(EqualityTester<false>(value1, value2));
            }
            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Copy));
            }
            WHEN("two different errors")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("mixed value and error")
            {
                CHECK(EqualityTester<false>(value1, error1));
            }
        }
        WHEN("compare with different type expected<T2, E2>")
        {
            using T2 = Type2;
            using E2 = Type1;
            static_assert(!std::is_same_v<T, T2>);
            static_assert(!std::is_same_v<E, E2>);
            using Expected2 = expected<T2, E2>;

            Expected const  value1     = "value1";
            Expected2 const value2     = "value2";
            Expected2 const value1Same = "value1";
            Expected const  error1     = zeus::unexpected<E>("error1");
            Expected2 const error2     = zeus::unexpected<E2>("error2");
            Expected2 const error1Same = zeus::unexpected<E2>("error1");

            WHEN("two same values")
            {
                CHECK(EqualityTester<true>(value1, value1Same));
            }
            WHEN("two different values")
            {
                CHECK(EqualityTester<false>(value1, value2));
            }
            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different errors")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("mixed value and errors")
            {
                CHECK(EqualityTester<false>(value1, error1));
            }
        }
        WHEN("compare with same value type T")
        {
            Expected const value1     = "value1";
            Expected const error1     = zeus::unexpected<E>("error1");
            T const        value2     = "value2";
            T const        value1Same = "value1";

            WHEN("two same values")
            {
                CHECK(EqualityTester<true>(value1, value1Same));
            }
            WHEN("two different values")
            {
                CHECK(EqualityTester<false>(value1, value2));
            }
            WHEN("lhs is error")
            {
                CHECK(EqualityTester<false>(error1, value2));
            }
        }
        WHEN("compare with different value type T2")
        {
            using T2 = Type2;
            static_assert(!std::is_same_v<T, T2>);

            Expected const value1     = "value1";
            Expected const error1     = zeus::unexpected<E>("error1");
            T2 const       value2     = "value2";
            T2 const       value1Same = "value1";

            WHEN("two same values")
            {
                CHECK(EqualityTester<true>(value1, value1Same));
            }
            WHEN("two different values")
            {
                CHECK(EqualityTester<false>(value1, value2));
            }
            WHEN("lhs is error")
            {
                CHECK(EqualityTester<false>(error1, value2));
            }
        }
        WHEN("compare with same error type unexpected<E>")
        {
            Expected const value1     = "value1";
            Expected const error1     = zeus::unexpected<E>("error1");
            auto const     error2     = zeus::unexpected<E>("error2");
            auto const     error1Same = zeus::unexpected<E>("error1");

            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different erros")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("lhs has value")
            {
                CHECK(EqualityTester<false>(value1, error2));
            }
        }
        WHEN("compare with different error type unexpected<E2>")
        {
            using E2 = Type1;
            static_assert(!std::is_same_v<E, E2>);

            Expected const value1     = "value1";
            Expected const error1     = zeus::unexpected<E>("error1");
            auto const     error2     = zeus::unexpected<E2>("error2");
            auto const     error1Same = zeus::unexpected<E2>("error1");

            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different erros")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("lhs has value")
            {
                CHECK(EqualityTester<false>(value1, error2));
            }
        }
    }
    GIVEN("expected<void, E>")
    {
        using E        = Type1;
        using Expected = expected<void, E>;

        WHEN("compare with same type expected<void, E>")
        {
            Expected const value1;
            Expected const value2;
            Expected const error1     = zeus::unexpected<E>("error1");
            Expected const error2     = zeus::unexpected<E>("error2");
            Expected const error1Copy = zeus::unexpected<E>("error1");

            WHEN("both have value")
            {
                CHECK(EqualityTester<true>(value1, value2));
            }
            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Copy));
            }
            WHEN("two different errors")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("mixed value and error")
            {
                CHECK(EqualityTester<false>(value1, error1));
            }
        }
        WHEN("compare with different type expected<void, E2>")
        {
            using E2 = Type2;
            static_assert(!std::is_same_v<E, E2>);
            using Expected2 = expected<void, E2>;

            Expected const  value1;
            Expected2 const value2;
            Expected const  error1     = zeus::unexpected<E>("error1");
            Expected2 const error2     = zeus::unexpected<E2>("error2");
            Expected2 const error1Same = zeus::unexpected<E2>("error1");

            WHEN("both have value")
            {
                CHECK(EqualityTester<true>(value1, value2));
            }
            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different errors")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("mixed value and errors")
            {
                CHECK(EqualityTester<false>(value1, error1));
            }
        }
        WHEN("compare with same error type unexpected<E>")
        {
            Expected const value1;
            Expected const error1     = zeus::unexpected<E>("error1");
            auto const     error2     = zeus::unexpected<E>("error2");
            auto const     error1Same = zeus::unexpected<E>("error1");

            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different erros")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("lhs has value")
            {
                CHECK(EqualityTester<false>(value1, error2));
            }
        }
        WHEN("compare with different error type unexpected<E2>")
        {
            using E2 = Type2;
            static_assert(!std::is_same_v<E, E2>);

            Expected const value1;
            Expected const error1     = zeus::unexpected<E>("error1");
            auto const     error2     = zeus::unexpected<E2>("error2");
            auto const     error1Same = zeus::unexpected<E2>("error1");

            WHEN("two same errors")
            {
                CHECK(EqualityTester<true>(error1, error1Same));
            }
            WHEN("two different erros")
            {
                CHECK(EqualityTester<false>(error1, error2));
            }
            WHEN("lhs has value")
            {
                CHECK(EqualityTester<false>(value1, error2));
            }
        }
    }
}
