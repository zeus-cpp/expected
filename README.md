# `zeus::expected`

Backporting `std::expected` to C++17.

## Features

Implemented C++ Standard Proposals:

- [x] [P0323R12](https://wg21.link/p0323r12) `<expected>`
- [x] [P2505R5](https://wg21.link/p2505r5) Monadic Functions For expected
- [x] [P2549R1](https://wg21.link/p2549r1) `std::unexpected<E>` should have `error()` as member accessor

Implemented LWG Issues:

- [x] [LWG-3836](https://wg21.link/lwg3836) `std::expected<bool, E1>` conversion constructor `expected(const expected<U, G>&)` should take precedence over `expected(U&&)` with `operator bool`
- [x] [LWG-3843](https://wg21.link/lwg3843) `std::expected<T,E>::value() &` assumes `E` is copy constructible
- [x] [LWG-3940](https://wg21.link/lwg3940) `std::expected<void, E>::value()` also needs `E` to be copy constructible
- [x] [LWG-4026](https://wg21.link/lwg4026) Assignment operators of `std::expected` should propagate triviality
- [x] [LWG-3877](https://wg21.link/lwg3877) incorrect constraints on const-qualified monadic overloads for `std::expected`

Enhancements:

+ Enhanced noexcept (covered by tests from MSVC's STL)

## Compiler supports

Any compiler that supports C++17 and later should work.
Usage against higher language standard is also supported, in which you'll gain some benefits (e.g. better constexpr).

While heavily tested under MSVC and C++20, it's only tested slightly under C++17, since newly-made.
May assume that it could just work if it compiled successfully.
Feedbacks are welcome.

Table of known compiler status.

| Planned             | Status          | Note                          |
| ------------------- | --------------- | ----------------------------- |
| MSVC v142 and later | Fully tested    | `/Zc:__cplusplus` is required |
| GCC 8               | Not compiled    |                               |
| GCC 9 and later     | Slightly tested |                               |

## Acknowledgements

+ [tl-expected](https://github.com/TartanLlama/expected), the original code base this library came from.
+ [MSVC's STL](https://github.com/microsoft/STL)'s massive, strong and robust tests.
