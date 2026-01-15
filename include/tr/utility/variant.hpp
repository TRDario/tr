///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous variant-related functionality.                                                                                 //
//                                                                                                                                       //
// A number of variant-related concepts are exposed:                                                                                     //
//     - tr::in_variant<int, std::variant<int, char, float>> -> true                                                                     //
//     - tr::in_variant<double, std::variant<int, char, float>> -> false                                                                 //
//     - tr::superset_of<std::variant<int, char, float>, std::variant<int, char>> -> true                                                //
//     - tr::superset_of<std::variant<int, char>, std::variant<int, char, float>> -> false                                               //
//     - tr::subset_of<std::variant<int, char>, std::variant<int, char, float>> -> true                                                  //
//     - tr::subset_of<std::variant<int, char, float>, std::variant<int, char>> -> false                                                 //
//                                                                                                                                       //
// A variant can be checked for holding one of a number of types:                                                                        //
//     - std::variant<int, char, float> v{5.0f}; tr::holds_one_of<int, float>(v) -> true                                                 //
//                                                                                                                                       //
// Variants which are subsets/supersets of each other can be cast between. Whether a superset is allowed to be cast with subset_case can //
// be checked with subset_castable_to:                                                                                                   //
//     - std::variant<int, char, float> v{5.0f}; tr::subset_castable_to<std::variant<int, char>>(v) -> false                             //
//     - std::variant<int, char, float> v{5.0f}; tr::subset_castable_to<std::variant<int, float>>(v) -> true                             //
//     - std::variant<int, char, float> v{5.0f}; tr::subset_cast<std::variant<int, float>>(v) -> std::variant<int, float>{5.0f}          //
//     - std::variant<int, float> v{5.0f}; tr::superset_cast<std::variant<int, char, float>>(v) -> std::variant<int, char, float>{5.0f}  //
//                                                                                                                                       //
// Indices of types in a variant can be gotten through tr::variant_indices:                                                              //
//     - using idx = tr::variant_indices<std::variant<int, char, float>>                                                                 //
//       switch (v.index()) {                                                                                                            //
//       case idx::get<int>:                                                                                                             //
//           tr::println("Int: {}", tr::get<int>(v));                                                                                    //
//           break;                                                                                                                      //
//       case idx::get<char>:                                                                                                            //
//           tr::println("Char: {}", tr::get<char>(v));                                                                                  //
//           break;                                                                                                                      //
//       case idx::get<float>:                                                                                                           //
//           tr::println("Float: {}", tr::get<float>(v));                                                                                //
//           break;                                                                                                                      //
//       }                                                                                                                               //
//                                                                                                                                       //
// An unchecked variant getter is provided through tr::get:                                                                              //
//     - std::variant<int, char, float> v{5.0f}; tr::get<float>(v) -> 5.0f                                                               //
//     - std::variant<int, std::string> v{"example"}; tr::get<std::string>(std::move(v)) -> rvalue reference to std::string{"example"}   //
//                                                                                                                                       //
// Stateless and stateful match classes are provided to support pseudo-match statement syntax. Lambdas passed to the stateful match must //
// take the state (by value or lvalue reference) as the first parameter. tr::ignore_other_cases or tr::default_result may be used for    //
// default handling of cases that aren't important:                                                                                      //
//     - std::variant<int, char, float> v{5.0f}                                                                                          //
//       v | tr::match{                                                                                                                  //
//           [] (float v) { tr::println("Float: {}", tr::get<float>(v)); },                                                              //
//           tr::ignore_other_cases                                                                                                      //
//       }                                                                                                                               //
//       -> "Float: 5.0"                                                                                                                 //
//     - std::variant<int, char, float> v{25}                                                                                            //
//       v | tr::stateful_match{                                                                                                         //
//           500,                                                                                                                        //
//           [] (int& state, int v) { return state + v; },                                                                               //
//           tr::default_result(42)                                                                                                      //
//       }                                                                                                                               //
//       -> 525                                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Determines whether a type is in a variant's type list.
	template <class T, specialization_of<std::variant> V> struct type_in_variant;
	// Determines whether a type is in a variant's type list.
	template <class T, class V>
	concept in_variant = type_in_variant<T, V>::value;
	// Gets whether a variant is holding one of a number of types.
	template <class... Ts, one_of<Ts...>... Us> constexpr bool holds_one_of(const std::variant<Us...>& v);

	// Determines whether a variant type is a superset of another variant type.
	template <specialization_of<std::variant> T, specialization_of<std::variant> V> struct is_superset_of;
	// Determines whether a variant type is a superset of another variant type.
	template <class T, class V>
	concept superset_of = is_superset_of<T, V>::value;
	// Determines whether a variant type is a subset of another variant type.
	template <class T, class V>
	concept subset_of = superset_of<V, T>;
	// Gets whether a variant can be casted using subset_cast to another type.
	template <class T, class V>
		requires(subset_of<T, V>)
	constexpr bool subset_castable_to(const V& v);
	// Casts a variant into another variant that is a superset of it.
	template <class To, class From>
		requires(superset_of<To, std::remove_cvref_t<From>>)
	constexpr To superset_cast(From&& v);
	// Casts a variant into another variant that is a subset of it.
	template <class To, class From>
		requires(subset_of<To, std::remove_cvref_t<From>>)
	constexpr To subset_cast(From&& v);

	// Struct which can be used to get the indices of types in a variant though get<T>.
	template <specialization_of<std::variant> T> struct variant_indices;

	// Unchecked variant getter.
	template <class T, class... Ts> T& get(std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> T&& get(std::variant<Ts...>&& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T& get(const std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T&& get(const std::variant<Ts...>&& v);

	// Match statement helper class.
	template <class... Fs> struct match : Fs... {
		using Fs::operator()...;
	};
	// Match statement.
	template <remove_cvref_specialization_of<std::variant> Variant, remove_cvref_specialization_of<match> Match>
	decltype(auto) operator|(Variant&& v, Match&& m);

	// Stateful match statement helper class.
	template <class State, class... Fs> class stateful_match : match<Fs...> {
	  public:
		// Constructs a stateful match statement.
		stateful_match(State&& state, Fs&&... fns);
		// Calls one of the underlying callables.
		template <class... Args> decltype(auto) operator()(Args&&... args);

	  private:
		// The match state.
		State m_state;
	};
	// Stateful match statement.
	template <remove_cvref_specialization_of<std::variant> Variant, remove_cvref_specialization_of<stateful_match> Match>
	decltype(auto) operator|(Variant&& v, Match&& m);

	// Used in a match statement to mark all other cases as ignorable.
	constexpr auto ignore_other_cases{[](auto&&...) {}};
	// Used in a match statement to give all other cases a default result.
	template <class T> constexpr auto default_result(T&& value);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "variant_impl.hpp"