#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

namespace
{
// Exposing all members for testing
struct test_bad_expected_access : public bad_expected_access<void>
{
    using bad_expected_access<void>::bad_expected_access;
};
}

TEST_CASE("bad_expected_access<void> special members are noexcept", "[LWG-4031]")
{
    STATIC_REQUIRE(std::is_nothrow_default_constructible_v<test_bad_expected_access>);
    STATIC_REQUIRE(std::is_nothrow_copy_constructible_v<test_bad_expected_access>);
    STATIC_REQUIRE(std::is_nothrow_move_constructible_v<test_bad_expected_access>);
    STATIC_REQUIRE(std::is_nothrow_copy_assignable_v<test_bad_expected_access>);
    STATIC_REQUIRE(std::is_nothrow_move_assignable_v<test_bad_expected_access>);
    STATIC_REQUIRE(std::is_nothrow_destructible_v<test_bad_expected_access>);
}
