/// @file
/// @brief Provides `tr::internal::binding_t` on Clang.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr::internal
{
#if TR_HAS_BUILTIN(__builtin_structured_binding_size)
	// clang-format off
	/// Gets the type of the `Index`th member of a structured binding of `Target`.
	/// @tparam Index Index of the structured binding member.
	/// @tparam Target Type being destructured.
	template <usize Index, typename Target>
		requires(Index < __builtin_structured_binding_size(std::remove_cvref_t<Target>))
	using binding_t = std::invoke_result_t<decltype([](Target&& v) -> decltype(auto) {
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wc++26-extensions"
		auto&& [... p]{std::forward<Target>(v)};
		return p...[Index];
		#pragma clang diagnostic pop
	}), Target>;
	// clang-format on
#endif
} // namespace tr::internal