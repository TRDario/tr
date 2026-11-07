/// @file
/// @brief Tests utility/arithmetic_concepts.hpp.

#include <tr/utility/arithmetic_concepts.hpp>

//

/// @brief Checks `tr::arithmetic` in the positive case.
static_assert(tr::arithmetic<int>, "int should fulfill tr::arithmetic, but does not!");

/// @brief Checks `tr::arithmetic` in the negative case.
static_assert(!tr::arithmetic<std::vector<int>>, "std::vector<int> should not fulfill tr::arithmetic, but does!");