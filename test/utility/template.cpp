///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/template.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility/template.hpp>

static_assert(tr::concatenate_string_literals("test", "tost", "tist") == std::string_view{"testtosttist"});

static_assert(tr::type_name<int>() == "int");
static_assert(tr::type_name<const float>() == "const float");

static_assert(std::same_as<tr::size_type_t<10>, tr::u8>);
static_assert(std::same_as<tr::size_type_t<1000>, tr::u16>);
static_assert(std::same_as<tr::size_type_t<100000>, tr::u32>);
static_assert(std::same_as<tr::size_type_t<10000000000>, tr::u64>);

template <int A, typename B> struct vt_template {};
static_assert(!tr::is_specialization_of<std::string, std::vector>::value);
static_assert(tr::is_specialization_of<std::vector<int>, std::vector>::value);
static_assert(!tr::is_specialization_of_v<std::string, std::ratio>::value);
static_assert(tr::is_specialization_of_v<std::ratio<1, 10>, std::ratio>::value);
static_assert(!tr::is_specialization_of_vt<std::string, vt_template>::value);
static_assert(tr::is_specialization_of_vt<vt_template<10, int>, vt_template>::value);
static_assert(!tr::is_specialization_of_tv<std::string, std::array>::value);
static_assert(tr::is_specialization_of_tv<std::array<int, 10>, std::array>::value);

struct method_examples {
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

constexpr bool return_true()
{
	return true;
}

static_assert([] {
	method_examples object;
	return tr::wrap_invocable(return_true)() && tr::wrap_invocable([](bool v) { return !v; })(false) &&
		   tr::wrap_invocable(&method_examples::test0)(object, 0, 0);
}());