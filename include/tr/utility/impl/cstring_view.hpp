///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of cstring_view.hpp.                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../cstring_view.hpp"
#include "../macro.hpp"

////////////////////////////////////////////////////////////// C-STRING VIEW //////////////////////////////////////////////////////////////

constexpr tr::cstring_view::cstring_view(const char* cstr)
	: m_cstr{cstr}
{
}

constexpr tr::cstring_view::cstring_view(const std::string& str)
	: cstring_view{str.c_str()}
{
}

//

constexpr bool tr::cstring_view::empty() const
{
	return m_cstr == nullptr;
}

//

constexpr tr::cstring_view::operator const char*() const
{
	return m_cstr;
}

constexpr tr::cstring_view::operator std::string_view() const
{
	TR_ASSERT(!empty(), "Cannot convert an empty C-string view to a string view.");

	return m_cstr;
}

constexpr tr::cstring_view::operator std::string() const
{
	TR_ASSERT(!empty(), "Cannot convert an empty C-string view to a string.");

	return m_cstr;
}