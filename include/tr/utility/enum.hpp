/// @file
/// @brief Provides enumerator utilities.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Wrapper around an enumerator to allow conversion to bool.
	/// @details This wrapper is primarily used in `TR_DEFINE_ENUM_BITMASK_OPERATORS`.
	/// @tparam Enum Wrapped enumerator type.
	template <typename Enum>
		requires std::is_enum_v<Enum>
	class enum_wrapper
	{
	  public:
		/// @brief Wraps an enumerator value.
		/// @param value Raw enumerator value.
		[[nodiscard]] constexpr enum_wrapper(Enum value) noexcept
			: m_value{value}
		{
		}

		//

		/// @brief Converts the enumerator to a boolean.
		/// @return Enumerator cast to a boolean.
		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return m_value != Enum{0};
		}

		/// @brief Unwraps the enumerator.
		/// @return Raw enumerator value.
		[[nodiscard]] constexpr operator Enum() const noexcept
		{
			return m_value;
		}

		/// @brief Unwraps the enumerator.
		/// @return Raw enumerator value.
		[[nodiscard]] constexpr Enum unwrap() const noexcept
		{
			return m_value;
		}

	  private:
		/// @brief Base enumerator value.
		Enum m_value;
	};
} // namespace tr

/// @ingroup meta
/// @brief Defines bitmask operators (&, |, ^, ~) for enum class types.
/// @param type Enumerator class type.
/// @hideinitializer
#define TR_DEFINE_ENUM_BITMASK_OPERATORS(type)                                                                                             \
	[[nodiscard]] constexpr tr::enum_wrapper<type> operator&(type lhs, type rhs) noexcept                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) & std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] constexpr tr::enum_wrapper<type> operator|(type lhs, type rhs) noexcept                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) | std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] constexpr tr::enum_wrapper<type> operator^(type lhs, type rhs) noexcept                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) ^ std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] constexpr tr::enum_wrapper<type> operator~(type lhs) noexcept                                                            \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~std::to_underlying(lhs))};                                                              \
	}                                                                                                                                      \
	constexpr type& operator&=(type& lhs, type rhs) noexcept                                                                               \
	{                                                                                                                                      \
		return lhs = (lhs & rhs);                                                                                                          \
	}                                                                                                                                      \
	constexpr type& operator|=(type& lhs, type rhs) noexcept                                                                               \
	{                                                                                                                                      \
		return lhs = (lhs | rhs);                                                                                                          \
	}                                                                                                                                      \
	constexpr type& operator^=(type& lhs, type rhs) noexcept                                                                               \
	{                                                                                                                                      \
		return lhs = (lhs ^ rhs);                                                                                                          \
	}

/// @ingroup meta
/// @brief Defines bitmask operators (&, |, ^, ~) for private enum class types of classes.
/// @details This macro must be invoked in the body of the class containing the enum class.
/// @param type Private enumerator class type.
/// @hideinitializer
#define TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(type)                                                                                     \
	[[nodiscard]] friend constexpr tr::enum_wrapper<type> operator&(type lhs, type rhs) noexcept                                           \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) & std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] friend constexpr tr::enum_wrapper<type> operator|(type lhs, type rhs) noexcept                                           \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) | std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] friend constexpr tr::enum_wrapper<type> operator^(type lhs, type rhs) noexcept                                           \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) ^ std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	[[nodiscard]] friend constexpr tr::enum_wrapper<type> operator~(type lhs) noexcept                                                     \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~std::to_underlying(lhs))};                                                              \
	}                                                                                                                                      \
	friend constexpr type& operator&=(type& lhs, type rhs) noexcept                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs & rhs);                                                                                                          \
	}                                                                                                                                      \
	friend constexpr type& operator|=(type& lhs, type rhs) noexcept                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs | rhs);                                                                                                          \
	}                                                                                                                                      \
	friend constexpr type& operator^=(type& lhs, type rhs) noexcept                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs ^ rhs);                                                                                                          \
	}
