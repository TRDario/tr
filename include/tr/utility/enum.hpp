///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides enumerator utilities.                                                                                                        //
//                                                                                                                                       //
// tr::to_underlying is an implementation of C++23 std::to_underlying.                                                                   //
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

namespace tr {
	// Wraps an enum class to allow conversion to bool, used in TR_DEFINE_ENUM_BITMASK_OPERATORS.
	template <enumerator Enum> class enum_wrapper {
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

	// Implements C++23 std::to_underlying.
	template <enumerator Enum> constexpr std::underlying_type_t<Enum> to_underlying(Enum enumerator);
} // namespace tr

#define TR_DEFINE_ENUM_BITMASK_OPERATORS(type)                                                                                             \
	constexpr tr::enum_wrapper<type> operator&(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) & tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator|(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) | tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator^(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) ^ tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator~(type l)                                                                                     \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~tr::to_underlying(l))};                                                                 \
	}                                                                                                                                      \
	constexpr type& operator&=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l & r);                                                                                                                \
	}                                                                                                                                      \
	constexpr type& operator|=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l | r);                                                                                                                \
	}                                                                                                                                      \
	constexpr type& operator^=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l ^ r);                                                                                                                \
	}

#define TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(type)                                                                                     \
	friend constexpr tr::enum_wrapper<type> operator&(type l, type r)                                                                      \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) & tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator|(type l, type r)                                                                      \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) | tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator^(type l, type r)                                                                      \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(tr::to_underlying(l) ^ tr::to_underlying(r))};                                           \
	}                                                                                                                                      \
	friend constexpr tr::enum_wrapper<type> operator~(type l)                                                                              \
	{                                                                                                                                      \
		return tr::enum_wrapper{static_cast<type>(~tr::to_underlying(l))};                                                                 \
	}                                                                                                                                      \
	friend constexpr type& operator&=(type& l, type r)                                                                                     \
	{                                                                                                                                      \
		return l = (l & r);                                                                                                                \
	}                                                                                                                                      \
	friend constexpr type& operator|=(type& l, type r)                                                                                     \
	{                                                                                                                                      \
		return l = (l | r);                                                                                                                \
	}                                                                                                                                      \
	friend constexpr type& operator^=(type& l, type r)                                                                                     \
	{                                                                                                                                      \
		return l = (l ^ r);                                                                                                                \
	}

#include "impl/enum.hpp" // IWYU pragma: export