/// @file
/// @brief Tests utility/apply.hpp.

#include <tr/utility/apply.hpp>

//

/// Tests the correctness of `tr::applicable` in a positive case.
static_assert(tr::applicable<void (*)(int, float, char), std::tuple<int, const float, char&>>,
			  "void(*)(int, float, char) should be applicable on std::tuple<int, const float, char&>, but isn't!");

/// Tests the correctness of `tr::applicable` in a positive case.
static_assert(tr::applicable<decltype([](int, float, char) {}), std::tuple<int, const float, char&>>,
			  "[](int, float, char) {} should be applicable on std::tuple<int, const float, char&>, but isn't!");

/// Tests the correctness of `tr::applicable` in a negative case.
static_assert(!tr::applicable<void (*)(int&, float&, char&), std::tuple<int, const float, char&>>,
			  "void(*)(int, float, char) should not be applicable on std::tuple<int, const float, char&>, but is!");

/// Tests the correctness of `tr::applicable` in a negative case.
static_assert(!tr::applicable<decltype([](int&, float&, char&) {}), std::tuple<int, const float, char&>>,
			  "[](int, float, char) {} should not be applicable on std::tuple<int, const float, char&>, but is!");

/// Tests the correctness of `tr::apply_result_t`.
static_assert(std::same_as<tr::apply_result_t<int (*)(int, float, char), std::tuple<int, const float, char&>>, int>,
			  "tr::apply_result_t<int (*)(int, float, char), std::tuple<int, const float, char&>> should be int, but isn't!");

/// Tests the correctness of `tr::apply_result_t`.
static_assert(std::same_as<tr::apply_result_t<decltype([](int, float, char) { return 0; }), std::tuple<int, const float, char&>>, int>,
			  "tr::apply_result_t<[](int, float, char) { return 0; }, std::tuple<int, const float, char&>> should be int, but isn't!");