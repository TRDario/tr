/// @file
/// @brief Tests utility/copy_qualifiers.hpp.

#include <tr/utility/copy_qualifiers.hpp>

//

/// Tests copying qualifiers from `float`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, float>, int>, "Failed to copy qualifiers from 'float'!");

/// Tests copying qualifiers from `const float`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, const float>, const int>, "Failed to copy qualifiers from 'const float'!");

/// Tests copying qualifiers from `float&`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, float&>, int&>, "Failed to copy qualifiers from 'float&'!");

/// Tests copying qualifiers from `const float&`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, const float&>, const int&>, "Failed to copy qualifiers from 'const float&'!");

/// Tests copying qualifiers from `float&&`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, float&&>, int&&>, "Failed to copy qualifiers from 'float&&'!");

/// Tests copying qualifiers from `const float&&`.
static_assert(std::same_as<tr::copy_qualifiers_t<int, const float&&>, const int&&>, "Failed to copy qualifiers from 'const float&&'!");