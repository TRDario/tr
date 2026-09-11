/// @file
/// @brief Provides enumerator utilities.

#pragma once
#include "concepts.hpp"

//

namespace tr
{
	/// Wraps an enum class to allow conversion to bool, used in `TR_DEFINE_ENUM_BITMASK_OPERATORS`.
	template <enumerator Enum>
	class enum_wrapper
	{
	  public:
		/// Wraps an enumerator value.
		/// @param value Raw enumerator value.
		[[nodiscard]] constexpr enum_wrapper(Enum value) noexcept;

		/// Converts the enumerator to a boolean.
		/// @return Enumerator cast to a boolean.
		[[nodiscard]] constexpr explicit operator bool() const noexcept;

		/// Unwraps the enumerator.
		/// @return Raw enumerator value.
		[[nodiscard]] constexpr operator Enum() const noexcept;

		/// Unwraps the enumerator.
		/// @return Raw enumerator value.
		[[nodiscard]] constexpr Enum unwrap() const noexcept;

	  private:
		/// Base enumerator value.
		Enum m_value;
	};
} // namespace tr

/// Defines bitmask operators (&, |, ^, ~) for enum class types.
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

/// Defines bitmask operators (&, |, ^, ~) for private enum class types of classes.
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

#include "impl/enum.hpp" // IWYU pragma: export