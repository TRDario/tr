///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Backports transform, and_then, and or_else for optional from C++23.                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Equivalent to C++23 std::optional.transform().
	template <typename In, std::invocable<In> Fn>
	constexpr std::optional<std::invoke_result_t<Fn, In&>> transform(std::optional<In>& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.transform().
	template <typename In, std::invocable<In> Fn>
	constexpr std::optional<std::invoke_result_t<Fn, const In&>> transform(const std::optional<In>& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.transform().
	template <typename In, std::invocable<In> Fn>
	constexpr std::optional<std::invoke_result_t<Fn, In>> transform(std::optional<In>&& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.transform().
	template <typename In, std::invocable<In> Fn>
	constexpr std::optional<std::invoke_result_t<Fn, const In>> transform(const std::optional<In>&& opt, Fn&& fn);

	// Equivalent to C++23 std::optional.and_then().
	template <typename In, std::invocable<In> Fn>
		requires(specialization_of<std::invoke_result_t<Fn, In&>, std::optional>)
	constexpr std::invoke_result_t<Fn, In&> and_then(std::optional<In>& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.and_then().
	template <typename In, std::invocable<In> Fn>
		requires(specialization_of<std::invoke_result_t<Fn, const In&>, std::optional>)
	constexpr std::invoke_result_t<Fn, const In&> and_then(const std::optional<In>& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.and_then().
	template <typename In, std::invocable<In> Fn>
		requires(specialization_of<std::invoke_result_t<Fn, In>, std::optional>)
	constexpr std::invoke_result_t<Fn, In> and_then(std::optional<In>&& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.and_then().
	template <typename In, std::invocable<In> Fn>
		requires(specialization_of<std::invoke_result_t<Fn, const In>, std::optional>)
	constexpr std::invoke_result_t<Fn, const In> and_then(const std::optional<In>&& opt, Fn&& fn);

	// Equivalent to C++23 std::optional.or_else().
	template <typename Element, std::invocable Fn>
		requires(std::same_as<std::invoke_result_t<Fn>, std::optional<Element>>)
	constexpr std::optional<Element> or_else(const std::optional<Element>& opt, Fn&& fn);
	// Equivalent to C++23 std::optional.or_else().
	template <typename Element, std::invocable Fn>
		requires(std::same_as<std::invoke_result_t<Fn>, std::optional<Element>>)
	constexpr std::optional<Element> or_else(std::optional<Element>&& opt, Fn&& fn);
} // namespace tr

#include "impl/optional.hpp"