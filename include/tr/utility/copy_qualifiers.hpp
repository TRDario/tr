/// @file
/// @brief Provides `tr::copy_qualifiers_t`.

#pragma once

//

namespace tr
{
	/// Type trait copying const/ref qualifiers from `Qualified` to `T`.
	/// @tparam Qualified Type holding the desired const/ref qualifiers.
	template <typename T, typename Qualified>
	struct copy_qualifiers
	{
		/// Holds `T` qualified with the same qualifiers as `Qualified`.
		/// @hideinitializer
		using type = decltype(std::forward_like<Qualified>(std::declval<std::remove_cvref_t<T>>()));
	};

	/// Copies const/ref qualifiers from `Qualified` to `T`.
	/// @tparam Qualified Type holding the desired const/ref qualifiers.
	template <typename T, typename Qualified>
	using copy_qualifiers_t = copy_qualifiers<T, Qualified>::type;
} // namespace tr