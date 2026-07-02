// LWG-4025 requires that operator=(expected&&) for expected<cv void, E> be
// constrained rather than conditionally deleted. The existing implementation
// already satisfies this via [class.copy.assign]/7 (CWG 1402): the base
// class's =delete causes expected<void,E>::operator=(expected&&) = default
// to be defined as deleted, and a defaulted move assignment that is defined
// as deleted is ignored by overload resolution — falling back to copy
// assignment when E is copyable but not movable.

#include <catch2/catch_all.hpp>

#include <zeus/expected.hpp>

using namespace zeus;

namespace
{

struct CopyableNotMovable
{
    CopyableNotMovable() = default;
    CopyableNotMovable(const CopyableNotMovable &) = default;
    CopyableNotMovable &operator=(const CopyableNotMovable &) = default;
    CopyableNotMovable(CopyableNotMovable &&) = delete;
    CopyableNotMovable &operator=(CopyableNotMovable &&) = delete;
};

struct NotCopyableNotMovable
{
    NotCopyableNotMovable() = default;
    NotCopyableNotMovable(const NotCopyableNotMovable &) = delete;
    NotCopyableNotMovable &operator=(const NotCopyableNotMovable &) = delete;
    NotCopyableNotMovable(NotCopyableNotMovable &&) = delete;
    NotCopyableNotMovable &operator=(NotCopyableNotMovable &&) = delete;
};

struct MoveOnly
{
    int value;
    MoveOnly() : value(0) {}
    MoveOnly(int v) : value(v) {}
    MoveOnly(const MoveOnly &) = delete;
    MoveOnly &operator=(const MoveOnly &) = delete;
    MoveOnly(MoveOnly &&) = default;
    MoveOnly &operator=(MoveOnly &&) = default;
};

struct StatefulCopyableNotMovable
{
    int value;
    StatefulCopyableNotMovable() : value(0) {}
    StatefulCopyableNotMovable(int v) : value(v) {}
    StatefulCopyableNotMovable(const StatefulCopyableNotMovable &) = default;
    StatefulCopyableNotMovable &operator=(const StatefulCopyableNotMovable &) = default;
    StatefulCopyableNotMovable(StatefulCopyableNotMovable &&) = delete;
    StatefulCopyableNotMovable &operator=(StatefulCopyableNotMovable &&) = delete;
};

} // namespace

TEST_CASE("expected<void, E> is move-assignable when E is movable", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<void, int>>);
}

TEST_CASE("expected<void, E> is trivially move-assignable when E is trivial", "[LWG-4025][LWG-4026]")
{
    STATIC_REQUIRE(std::is_trivially_move_assignable_v<expected<void, int>>);
    STATIC_REQUIRE(std::is_nothrow_move_assignable_v<expected<void, int>>);
}

TEST_CASE("expected<void, E> is move-assignable via copy fallback when E is copyable but not movable",
          "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<void, CopyableNotMovable>>);
    STATIC_REQUIRE(std::is_trivially_move_assignable_v<expected<void, CopyableNotMovable>>);
    STATIC_REQUIRE(std::is_nothrow_move_assignable_v<expected<void, CopyableNotMovable>>);
}

TEST_CASE("expected<void, E> copy fallback: both sides have value", "[LWG-4025]")
{
    expected<void, CopyableNotMovable> e1;
    expected<void, CopyableNotMovable> e2;
    e2 = std::move(e1);
    CHECK(e2.has_value());
}

TEST_CASE("expected<void, E> copy fallback: this has error, rhs has value", "[LWG-4025]")
{
    expected<void, CopyableNotMovable> e1;
    expected<void, CopyableNotMovable> e2(unexpect);
    e2 = std::move(e1);
    CHECK(e2.has_value());
}

TEST_CASE("expected<void, E> copy fallback: both sides have error", "[LWG-4025]")
{
    expected<void, CopyableNotMovable> e1(unexpect);
    expected<void, CopyableNotMovable> e2(unexpect);
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
}

TEST_CASE("expected<void, E> is not move-assignable when E is neither copyable nor movable", "[LWG-4025]")
{
    STATIC_REQUIRE_FALSE(std::is_move_assignable_v<expected<void, NotCopyableNotMovable>>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<expected<void, NotCopyableNotMovable>>);
}

TEST_CASE("expected<void, E> is copy-assignable when E is copyable but not movable", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_copy_assignable_v<expected<void, CopyableNotMovable>>);
}

TEST_CASE("expected<void, E> is move-assignable but not copy-assignable when E is move-only", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<void, MoveOnly>>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<expected<void, MoveOnly>>);
}

TEST_CASE("expected<void, E> move assignment works when E is movable", "[LWG-4025]")
{
    expected<void, int> e1(unexpect, 42);
    expected<void, int> e2;
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error() == 42);
}

TEST_CASE("expected<void, E> move assignment works when E is move-only", "[LWG-4025]")
{
    expected<void, MoveOnly> e1(unexpect, 42);
    expected<void, MoveOnly> e2;
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error().value == 42);
}

TEST_CASE("expected<void, E> copy fallback: this has value, rhs has error", "[LWG-4025]")
{
    expected<void, CopyableNotMovable> e1(unexpect);
    expected<void, CopyableNotMovable> e2;
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
}

TEST_CASE("expected<void, E> copy fallback propagates error value", "[LWG-4025]")
{
    expected<void, StatefulCopyableNotMovable> e1(unexpect, 42);
    expected<void, StatefulCopyableNotMovable> e2;
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error().value == 42);
}

TEST_CASE("expected<void, E> copy fallback propagates error value (both have error)", "[LWG-4025]")
{
    expected<void, StatefulCopyableNotMovable> e1(unexpect, 10);
    expected<void, StatefulCopyableNotMovable> e2(unexpect, 20);
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error().value == 10);
}

TEST_CASE("expected<T, E> is move-assignable when E is movable", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<int, int>>);
}

TEST_CASE("expected<T, E> is trivially move-assignable when E is trivial", "[LWG-4025][LWG-4026]")
{
    STATIC_REQUIRE(std::is_trivially_move_assignable_v<expected<int, int>>);
    STATIC_REQUIRE(std::is_nothrow_move_assignable_v<expected<int, int>>);
}

TEST_CASE("expected<T, E> is move-assignable via copy fallback when E is copyable but not movable",
          "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<int, CopyableNotMovable>>);
    STATIC_REQUIRE(std::is_trivially_move_assignable_v<expected<int, CopyableNotMovable>>);
    STATIC_REQUIRE(std::is_nothrow_move_assignable_v<expected<int, CopyableNotMovable>>);
}

TEST_CASE("expected<T, E> is not move-assignable when E is neither copyable nor movable", "[LWG-4025]")
{
    STATIC_REQUIRE_FALSE(std::is_move_assignable_v<expected<int, NotCopyableNotMovable>>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<expected<int, NotCopyableNotMovable>>);
}

TEST_CASE("expected<T, E> is copy-assignable when E is copyable but not movable", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_copy_assignable_v<expected<int, CopyableNotMovable>>);
}

TEST_CASE("expected<T, E> is move-assignable but not copy-assignable when E is move-only", "[LWG-4025]")
{
    STATIC_REQUIRE(std::is_move_assignable_v<expected<int, MoveOnly>>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<expected<int, MoveOnly>>);
}

TEST_CASE("expected<T, E> move assignment works when E is movable", "[LWG-4025]")
{
    expected<int, int> e1(unexpect, 42);
    expected<int, int> e2;
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error() == 42);
}

TEST_CASE("expected<T, E> copy fallback: both sides have value", "[LWG-4025]")
{
    expected<int, CopyableNotMovable> e1(100);
    expected<int, CopyableNotMovable> e2(200);
    e2 = std::move(e1);
    CHECK(e2.has_value());
    CHECK(*e2 == 100);
}

TEST_CASE("expected<T, E> copy fallback: this has error, rhs has value", "[LWG-4025]")
{
    expected<int, CopyableNotMovable> e1(100);
    expected<int, CopyableNotMovable> e2(unexpect);
    e2 = std::move(e1);
    CHECK(e2.has_value());
    CHECK(*e2 == 100);
}

TEST_CASE("expected<T, E> copy fallback: this has value, rhs has error", "[LWG-4025]")
{
    expected<int, CopyableNotMovable> e1(unexpect);
    expected<int, CopyableNotMovable> e2(200);
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
}

TEST_CASE("expected<T, E> copy fallback: both sides have error", "[LWG-4025]")
{
    expected<int, CopyableNotMovable> e1(unexpect);
    expected<int, CopyableNotMovable> e2(unexpect);
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
}

TEST_CASE("expected<T, E> copy fallback propagates error value", "[LWG-4025]")
{
    expected<int, StatefulCopyableNotMovable> e1(unexpect, 42);
    expected<int, StatefulCopyableNotMovable> e2(100);
    e2 = std::move(e1);
    CHECK_FALSE(e2.has_value());
    CHECK(e2.error().value == 42);
}

TEST_CASE("expected<T, E> copy fallback propagates value", "[LWG-4025]")
{
    expected<int, StatefulCopyableNotMovable> e1(999);
    expected<int, StatefulCopyableNotMovable> e2(unexpect, 10);
    e2 = std::move(e1);
    CHECK(e2.has_value());
    CHECK(*e2 == 999);
}
