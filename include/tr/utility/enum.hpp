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
		constexpr enum_wrapper(Enum value);

		/// Converts the enumerator to a boolean.
		/// @return Enumerator cast to a boolean.
		constexpr explicit operator bool() const;

		/// Unwraps the enumerator.
		/// @return Raw enumerator value.
		constexpr operator Enum() const;

		/// Unwraps the enumerator.
		/// @return Raw enumerator value.
		constexpr Enum unwrap() const;

	  private:
		/// Base enumerator value.
		Enum m_value;
	};
} // namespace tr

/// Defines bitmask operators (&, |, ^, ~) for enum class types.
/// @param type Enumerator class type.
/// @hideinitializer
#define TR_DEFINE_ENUM_BITMASK_OPERATORS(type)                                                                                             \
	constexpr tr::enum_wrapper<type> operator&(type lhs, type rhs)                                                                         \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) & std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator|(type lhs, type rhs)                                                                         \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) | std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator^(type lhs, type rhs)                                                                         \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) ^ std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator~(type lhs)                                                                                   \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~std::to_underlying(lhs))};                                                              \
	}                                                                                                                                      \
	constexpr type& operator&=(type& lhs, type rhs)                                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs & rhs);                                                                                                          \
	}                                                                                                                                      \
	constexpr type& operator|=(type& lhs, type rhs)                                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs | rhs);                                                                                                          \
	}                                                                                                                                      \
	constexpr type& operator^=(type& lhs, type rhs)                                                                                        \
	{                                                                                                                                      \
		return lhs = (lhs ^ rhs);                                                                                                          \
	}

/// Defines bitmask operators (&, |, ^, ~) for private enum class types of classes.
/// @param type Private enumerator class type.
/// @hideinitializer
#define TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(type)                                                                                     \
	friend constexpr tr::enum_wrapper<type> operator&(type lhs, type rhs)                                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) & std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator|(type lhs, type rhs)                                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) | std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator^(type lhs, type rhs)                                                                  \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(std::to_underlying(lhs) ^ std::to_underlying(rhs))};                                     \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator~(type lhs)                                                                            \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~std::to_underlying(lhs))};                                                              \
	}                                                                                                                                      \
	friend constexpr type& operator&=(type& lhs, type rhs)                                                                                 \
	{                                                                                                                                      \
		return lhs = (lhs & rhs);                                                                                                          \
	}                                                                                                                                      \
	friend constexpr type& operator|=(type& lhs, type rhs)                                                                                 \
	{                                                                                                                                      \
		return lhs = (lhs | rhs);                                                                                                          \
	}                                                                                                                                      \
	friend constexpr type& operator^=(type& lhs, type rhs)                                                                                 \
	{                                                                                                                                      \
		return lhs = (lhs ^ rhs);                                                                                                          \
	}

#include "impl/enum.hpp" // IWYU pragma: export