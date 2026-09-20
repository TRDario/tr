/// @file
/// @brief Provides `tr::optional_result`.

#pragma once
#include <tr/utility/result.hpp>

//

namespace tr
{
	template <specialization_of<result_values> Values, specialization_of<result_errors> Errors>
	class optional_result;

	/// Optional result variant with distinct value and error alternatives.
	/// @tparam Values Value alternatives.
	/// @tparam Errors Error alternatives.
	template <typename... Values, typename... Errors>
		requires((!one_of<Values, Errors...>) && ...)
	class optional_result<result_values<Values...>, result_errors<Errors...>> : public optional_variant<Values..., Errors...>
	{
	  public:
		/// @name Constants
		/// @{

		/// Holds whether a type is a value type.
		/// @tparam T Type to check.
		template <typename T>
		static constexpr bool is_value_type{one_of<T, Values...>};

		/// Holds whether a type is a error type.
		/// @tparam T Type to check.
		template <typename T>
		static constexpr bool is_error_type{one_of<T, Errors...>};

		/// @}
		/// @name Constructors and destructors
		/// @{

		using optional_variant<Values..., Errors...>::optional_variant;

		/// @}
		/// @name Error
		/// @{

		/// Gets if the variant is empty.
		/// @return `true` if the variant is empty, `false` otherwise.
		[[nodiscard]] constexpr bool is_empty() const noexcept
		{
			return !std::holds_alternative<empty_variant_t>(*this);
		}

		/// Gets if the variant holds a value type.
		/// @return `true` if the variant holds a value type, `false` otherwise.
		[[nodiscard]] constexpr bool is_value() const noexcept
		{
			return (std::holds_alternative<Values>(*this) || ...);
		}

		/// Gets if the variant holds an error type.
		/// @return `true` if the variant holds an error type, `false` otherwise.
		[[nodiscard]] constexpr bool is_error() const noexcept
		{
			return (std::holds_alternative<Errors>(*this) || ...);
		}

		/// @}
	  private:
		using optional_variant<Values..., Errors...>::has_value;
	};
} // namespace tr