#include <zeus/expected.hpp>

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
static_assert(false, "This test must be compiled with exceptions disabled");
#endif

zeus::expected<int, int> try_parse(bool succeed)
{
    if (succeed)
        return 42;
    return zeus::unexpected(1);
}

int main()
{
    auto e = try_parse(true);
    if (!e.has_value())
        return 1;
    if (*e != 42)
        return 2;

    auto e2 = try_parse(false);
    if (e2.has_value())
        return 3;
    if (e2.error() != 1)
        return 4;

    if (e2.value_or(99) != 99)
        return 5;

    auto e3 = e.and_then([](int v) -> zeus::expected<int, int> { return v + 1; });
    if (*e3 != 43)
        return 6;

    auto e4 = e2.or_else([](int) -> zeus::expected<int, int> { return 0; });
    if (*e4 != 0)
        return 7;

    zeus::expected<void, int> ev;
    if (!ev.has_value())
        return 8;

    zeus::expected<void, int> ev2{zeus::unexpect, 10};
    if (ev2.has_value())
        return 9;
    if (ev2.error() != 10)
        return 10;

    return 0;
}
