/// @file
/// @brief Provides `tr::result`.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/optional_variant.hpp>
#include <tr/utility/specialization_of.hpp>

//

namespace tr
{
	/// List of value alternatives for `tr::result`.
	/// @tparam Alternatives Value alternatives.
	template <typename... Alternatives>
	struct result_values
	{
	};

	/// List of error alternatives for `tr::result`.
	/// @tparam Alternatives Error alternatives.
	template <typename... Alternatives>
	struct result_errors
	{
	};

	template <specialization_of<result_values> Values, specialization_of<result_errors> Errors>
	class result;

	/// Result variant with distinct value and error alternatives.
	/// @tparam Values Value alternatives.
	/// @tparam Errors Error alternatives.
	template <typename... Values, typename... Errors>
		requires((!one_of<Values, Errors...>) && ...)
	class result<result_values<Values...>, result_errors<Errors...>> : public variant<Values..., Errors...>
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

		using variant<Values..., Errors...>::variant;

		/// @}
		/// @name Error
		/// @{

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
	};
} // namespace tr