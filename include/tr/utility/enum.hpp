///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides enumerator utilities.                                                                                                        //
//                                                                                                                                       //
// TR_DEFINE_ENUM_BITMASK_OPERATORS(type) defines bitmask operators (&, |, ^, ~) for enum class types,                                   //
// while TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(type) does the same for private enums in classes:                                      //
//     - enum class my_enum {                                                                                                            //
//           l = 1,                                                                                                                      //
//           r = 2,                                                                                                                      //
//           lr = 3                                                                                                                      //
//       };                                                                                                                              //
//       TR_DEFINE_ENUM_BITMASK_OPERATORS(my_enum);                                                                                      //
//     - my_enum::lr & my_enum::r -> my_enum::r                                                                                          //
//     - my_enum::l | my_enum::r -> my_enum::lr                                                                                          //
//     - my_enum::lr ^ my_enum::r -> my_enum::l                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

namespace tr
{
	// Wraps an enum class to allow conversion to bool, used in TR_DEFINE_ENUM_BITMASK_OPERATORS.
	template <enumerator Enum>
	class enum_wrapper
	{
	  public:
		// Wraps an enumerator value.
		constexpr enum_wrapper(Enum value);

		// Converts the enumerator to a boolean.
		constexpr explicit operator bool() const;

		// Unwraps the enumerator.
		constexpr operator Enum() const;
		// Unwraps the enumerator.
		constexpr Enum unwrap() const;

	  private:
		// The wrapped enumerator value.
		Enum m_value;
	};
} // namespace tr

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