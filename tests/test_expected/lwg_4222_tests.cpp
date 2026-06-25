#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

namespace
{

struct ConstructibleFromUnexpect
{
    ConstructibleFromUnexpect() = default;
    explicit ConstructibleFromUnexpect([[maybe_unused]] unexpect_t ut) {}
};

} // namespace

TEST_CASE("expected<T, E>(unexpect) is not ambiguous when T is constructible from unexpect_t", "[LWG-4222]")
{
    expected<ConstructibleFromUnexpect, int> e(unexpect);
    CHECK_FALSE(e.has_value());
    CHECK(e.error() == int {});
}

TEST_CASE("expected<T, E>(unexpect, args) still works when T is constructible from unexpect_t", "[LWG-4222]")
{
    expected<ConstructibleFromUnexpect, int> e(unexpect, 42);
    CHECK_FALSE(e.has_value());
    CHECK(e.error() == 42);
}
