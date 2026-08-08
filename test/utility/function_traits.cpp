/// @file
/// @brief Tests function_traits.hpp.

#include <tr/utility/function_traits.hpp>

//

struct method_examples
{
	constexpr int test0(float, float)
	{
		return true;
	}

	int test1() const;
	int test2() &;
	int test3() const&;
	int test4() &&;
	int test5() const&&;
};

//

static_assert(std::same_as<tr::return_type_t<int(float, int)>, int>);

static_assert(std::same_as<tr::return_type_t<int (*)(float, int)>, int>);

static_assert(std::same_as<tr::return_type_t<int (&)(float, int)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test0)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test1)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test2)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test3)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test4)>, int>);

static_assert(std::same_as<tr::return_type_t<decltype(&method_examples::test5)>, int>);

static_assert(std::same_as<tr::class_type_t<decltype(&method_examples::test0)>, method_examples>);

static_assert(std::same_as<tr::nth_arg_type_t<0, int(float, int)>, float>);

static_assert(std::same_as<tr::nth_arg_type_t<0, int (*)(float, int)>, float>);

static_assert(std::same_as<tr::nth_arg_type_t<0, int (&)(float, int)>, float>);

static_assert(std::same_as<tr::nth_arg_type_t<0, decltype(&method_examples::test0)>, float>);

static_assert(std::same_as<tr::nth_arg_type_t<1, int(float, int)>, int>);