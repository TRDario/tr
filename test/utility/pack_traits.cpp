/// @file
/// @brief Tests utility/pack_traits.hpp.

#include <tr/utility/pack_traits.hpp>

//

/// Tests `tr::pack_front_t`.
static_assert(std::same_as<tr::pack_front_t<int, float, char>, int>, "tr::pack_front_t<int, float, char> should be int, but is not!");

/// Tests `tr::pack_index_t`.
static_assert(std::same_as<tr::pack_index_t<0, int, float, char>, int>, "tr::pack_index_t<0, int, float, char> should be int, but is not!");

/// Tests `tr::pack_index_t`.
static_assert(std::same_as<tr::pack_index_t<1, int, float, char>, float>,
			  "tr::pack_index_t<1, int, float, char> should be float, but is not!");

//

/// Tests `tr::one_of` on a positive case.
static_assert(tr::one_of<int, int, float, char>, "int should be contained in <int, float, char>, but is not!");

/// Tests `tr::one_of` on a negative case.
static_assert(!tr::one_of<double, int, float, char>, "double should not be contained in <int, float, char>, but is!");

/// Tests `tr::all_same` on a positive case.
static_assert(tr::all_same<int, int, int>, "<int, int, int> should be counted as all the same, but is not!");

/// Tests `tr::all_same` on a negative case.
static_assert(!tr::all_same<int, float, char>, "<int, float, char> should not be counted as all the same, but is!");