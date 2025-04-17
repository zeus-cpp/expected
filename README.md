# `zeus::expected`

[![Tests Workflow](https://github.com/zeus-cpp/expected/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/zeus-cpp/expected/actions/workflows/tests.yml?query=branch%3Amain)
[![GitHub Release](https://img.shields.io/github/v/release/zeus-cpp/expected?color=green)](https://github.com/zeus-cpp/expected/releases)
[![Conan Center](https://img.shields.io/conan/v/zeus_expected)](https://conan.io/center/recipes/zeus_expected)

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
- [x] [LWG-3886](https://wg21.link/lwg3886) Monad mo' problems

Enhancements:

+ Enhanced noexcept (covered by tests from MSVC's STL)

## Compiler supports

Any compiler that supports C++17 should work.

Higher language standards are also supported, which can provide benefits such as enhanced constexpr capabilities.

List of known compiler supported:

+ MSVC v142 and later
+ GCC 8 and later

Feedbacks are welcome.

## Building and testing

[Catch2](https://github.com/catchorg/Catch2) is required to build the tests.
You may choose any preferred package manager to introduce the requirements. Here is a `conan` way.

```bash
# install the requirements
conan install . -s build_type=Debug -b missing
# configure, build and test with cmake
# ...
```

## Acknowledgements

+ [tl-expected](https://github.com/TartanLlama/expected), the original code base this library came from.
+ [MSVC's STL](https://github.com/microsoft/STL)'s massive, strong and robust tests.
