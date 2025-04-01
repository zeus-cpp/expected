#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

namespace
{

struct ConstructModeDistinction
{
    enum ConstructedMode
    {
        kDefault,
        kCopy,
        kMove,
        kCopyAssign,
        kMoveAssign,
    };

    constexpr ConstructModeDistinction() noexcept
        : mode(kDefault)
    {
    }
    constexpr ConstructModeDistinction(ConstructModeDistinction const&) noexcept
        : mode(kCopy)
    {
    }
    constexpr ConstructModeDistinction(ConstructModeDistinction&&) noexcept
        : mode(kMove)
    {
    }
    constexpr ConstructModeDistinction& operator=(ConstructModeDistinction&&) noexcept
    {
        mode = kMoveAssign;
        return *this;
    }
    constexpr ConstructModeDistinction& operator=(ConstructModeDistinction const&) noexcept
    {
        mode = kCopyAssign;
        return *this;
    }

    ConstructedMode mode;
};

} // namespace

SCENARIO("Monad mo' problems", "[LWG-3886]")
{
    using T = const ConstructModeDistinction;
    using E = int;

    // FIXME LWG-3891
    //SECTION("constructor")
    //{
    //    expected<T, E> const e({});
    //    CHECK(e.value().mode == T::kMove);
    //}
    //SECTION("assignment")
    //{
    //    expected<T, E> e {zeus::unexpect};
    //    e = {};
    //    CHECK(e.value().mode == T::kMove);
    //}

    SECTION("value_or()")
    {
        expected<T, E> const e {zeus::unexpect};
        CHECK(e.value_or({}).mode == T::kMove);
    }
}
