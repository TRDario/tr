///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous variant-related functionality.                                                                                 //
//                                                                                                                                       //
// Wrappers over std::get_if that take and return optionals are provided through tr::get_if:                                             //
//     - std::variant<int, char, float> v{5.0f}; tr::get_if<float>(v) -> 5.0f                                                            //
//     - std::variant<int, char, float> v{5.0f}; tr::get_if<char>(v) -> std::nullopt                                                     //
//                                                                                                                                       //
// Functions that call a function if a variant contains a specific type are provided through tr::if_is:                                  //
//     - std::variant<int, char, float> v{5.0f}; tr::if_is<int>(v, [](int v) { tr::println("{}", v); }) -> doesn't output anything       //
//     - std::variant<int, char, float> v{5.0f}; tr::if_is<float>(v, [](float v) { tr::println("{}", v); }) -> "5.0"                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"
#include "reference.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Wrapper over std::get_if.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr opt_ref<Alternative> get_if(std::variant<Alternatives...>& v);
	// Gets a value if the variant contains it.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr std::optional<Alternative> get_if(std::variant<Alternatives...>&& v);
	// Wrapper over std::get_if.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr opt_ref<const Alternative> get_if(const std::variant<Alternatives...>& v);
	// Gets a value if the variant contains it.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr std::optional<const Alternative> get_if(const std::variant<Alternatives...>&& v);

	// Executes a function if the variant holds a specific type.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative&> Fn>
	constexpr void if_is(std::variant<Alternatives...>& v, Fn&& fn);
	// Executes a function if the variant holds a specific type.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative> Fn>
	constexpr void if_is(std::variant<Alternatives...>&& v, Fn&& fn);
	// Executes a function if the variant holds a specific type.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative&> Fn>
	constexpr void if_is(const std::variant<Alternatives...>& v, Fn&& fn);
	// Executes a function if the variant holds a specific type.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative> Fn>
	constexpr void if_is(const std::variant<Alternatives...>&& v, Fn&& fn);
} // namespace tr

#include "impl/variant.hpp" // IWYU pragma: export