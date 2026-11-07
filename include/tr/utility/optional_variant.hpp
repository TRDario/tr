/// @file
/// @brief Provides `tr::optional_variant`.

#pragma once
#include <tr/utility/variant.hpp>

//

namespace tr
{
	/// @ingroup variant
	/// @brief Empty variant tag type.
	struct empty_variant_t
	{
	};

	/// @ingroup variant
	/// @brief Empty variant tag value.
	inline constexpr empty_variant_t empty_variant;

	//

	/// @ingroup variant
	/// @brief Variant with a distinct 'empty' state.
	/// @details The empty state is represented with `empty_variant_t` when visiting.
	/// @tparam Alternatives Variant alternatives.
	template <typename... Alternatives>
	class optional_variant : public variant<empty_variant_t, Alternatives...>
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		using variant<empty_variant_t, Alternatives...>::variant;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Resets the variant into an empty state.
		void reset() noexcept
		{
			this->template emplace<empty_variant_t>();
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the variant holds a value.
		/// @return `true` if the variant holds a value, `false` otherwise.
		[[nodiscard]] constexpr operator bool() const noexcept
		{
			return !std::holds_alternative<empty_variant_t>(*this);
		}

		/// @brief Checks whether the variant holds a value.
		/// @return `true` if the variant holds a value, `false` otherwise.
		[[nodiscard]] constexpr bool has_value() const noexcept
		{
			return !std::holds_alternative<empty_variant_t>(*this);
		}

		/// @}
	};
} // namespace tr