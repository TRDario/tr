/// @file
/// @brief Provides `tr::optional_variant`.

#pragma once
#include <tr/utility/variant.hpp>

//

namespace tr
{
	/// Empty variant tag type.
	struct empty_variant_t
	{
	};

	/// Empty variant tag value.
	inline constexpr empty_variant_t empty_variant;

	//

	/// Variant that may be empty.
	/// @details The empty state is represented with `empty_variant_t` when visiting.
	/// @tparam Alternatives Variant alternatives.
	template <typename... Alternatives>
	class optional_variant : public variant<empty_variant_t, Alternatives...>
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		using variant<empty_variant_t, Alternatives...>::variant;

		/// @}
		/// @name Value
		/// @{

		/// Gets if the variant holds a value.
		/// @return `true` if the variant holds a value, `false` otherwise.
		[[nodiscard]] constexpr bool has_value() const noexcept
		{
			return std::holds_alternative<empty_variant_t>(*this);
		}

		/// Resets the variant into an empty state.
		void reset() noexcept
		{
			this->template emplace<empty_variant_t>();
		}

		/// @}
	};
} // namespace tr