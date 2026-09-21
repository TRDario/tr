/// @file
/// @brief Provides `tr::internal::binding_t` on Clang.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr::internal
{
#ifdef __clang__
	// clang-format off
    /// Gets the type of the `I`th member of a structured binding of `T`.
    /// @tparam I Index of the structured binding member.
    /// @tparam T Type being destructured.
	template <usize I, typename T>
		requires(I < __builtin_structured_binding_size(std::remove_cvref_t<T>))
	using binding_t = std::invoke_result_t<decltype([](T&& v) -> decltype(auto) {
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wc++26-extensions"
		auto&& [... p]{std::forward<T>(v)};
		return p...[I];
        #pragma clang diagnostic pop
	}), T>;
	// clang-format on
#endif
} // namespace tr::internal